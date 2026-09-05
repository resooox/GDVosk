/*
 * Copyright 2026 Reid Smith
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/worker_thread_pool.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "speech_recognizer.hpp"

using namespace godot;

SpeechRecognizer::SpeechRecognizer() {
    recognizer = nullptr;
    sample_rate = 16000.0f;
}

SpeechRecognizer::~SpeechRecognizer() {
    if (recognizer != nullptr) {
        vosk_recognizer_free(recognizer);
        recognizer = nullptr;
    }
    speech_model.unref();
}

void SpeechRecognizer::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_sample_rate"), &SpeechRecognizer::get_sample_rate);
    ClassDB::bind_method(D_METHOD("set_sample_rate"), &SpeechRecognizer::set_sample_rate);
    
    ClassDB::bind_method(D_METHOD("get_language_model"), &SpeechRecognizer::get_language_model);
    ClassDB::bind_method(D_METHOD("set_language_model", "model"), &SpeechRecognizer::set_language_model);

    ClassDB::bind_method(D_METHOD("get_keywords"), &SpeechRecognizer::get_keywords);
    ClassDB::bind_method(D_METHOD("set_keywords", "words"), &SpeechRecognizer::set_keywords);

    ClassDB::bind_method(D_METHOD("send_waveform_pcm", "data"), &SpeechRecognizer::send_waveform_pcm);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "language_model", PROPERTY_HINT_RESOURCE_TYPE, "SpeechModel"), "set_language_model", "get_language_model");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "sample_rate"), "set_sample_rate", "get_sample_rate");

    ADD_SIGNAL(MethodInfo("result", PropertyInfo(Variant::STRING, "text")));
    ADD_SIGNAL(MethodInfo("partial_result", PropertyInfo(Variant::STRING, "text")));
}

void SpeechRecognizer::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_READY:
            init_recognizer();
            break;
    }
}

void SpeechRecognizer::init_recognizer() {
    if (recognizer != nullptr) {
        vosk_recognizer_free(recognizer);
        recognizer = nullptr;
    }

    if (speech_model.is_null()) {
        return;
    }

    VoskModel *vosk_model = speech_model->get_model();
    ERR_FAIL_NULL_MSG(vosk_model, "Failed to get Vosk model.");

    VoskRecognizer *vosk_recognizer = vosk_recognizer_new(vosk_model, sample_rate);
    ERR_FAIL_NULL_MSG(vosk_recognizer, "Failed to create Vosk Recognizer.");

    if (!keywords.is_empty()) {
        PackedStringArray parts;
        parts.resize(keywords.size());
        for (int i = 0; i < keywords.size(); i++) {
            parts[i] = "\"" + keywords[i].to_lower() + "\"";
        }
        String grammar = "[" + String(",").join(parts) + ",\"[unk]\"]";
        vosk_recognizer_set_grm(vosk_recognizer, grammar.utf8().get_data());
    }
    last_partial = "";
    recognizer = vosk_recognizer;
}

void SpeechRecognizer::send_waveform_pcm(const PackedByteArray &data) {
    if (data.is_empty()) {
        return;
    }

    if (recognizer != nullptr) {
        if (vosk_recognizer_accept_waveform(recognizer, reinterpret_cast<const char *>(data.ptr()), data.size())) {
            const char *result = vosk_recognizer_result(recognizer);
            if (result != nullptr) {
                call_deferred("emit_signal", "result", String::utf8(result));
            }
            last_partial = "";
        } else {
            String current_partial = String::utf8(vosk_recognizer_partial_result(recognizer));
            Ref<JSON> json;
            json.instantiate();
            Error err = json->parse(current_partial);
            if (err == OK) {
                Dictionary result = json->get_data();
                if (result.has("partial")) {
                    String partial_text = result["partial"];
                    if (partial_text != last_partial) {
                        last_partial = partial_text;
                        call_deferred("emit_signal", "partial_result", partial_text);
                    }
                }
            }
        }
    }
}

Ref<SpeechModel> SpeechRecognizer::get_language_model() const {
    return speech_model;
}

void SpeechRecognizer::set_language_model(Ref<SpeechModel> model) {
    speech_model = model;
    init_recognizer();
}

PackedStringArray SpeechRecognizer::get_keywords() const {
    return keywords;
}

void SpeechRecognizer::set_keywords(const PackedStringArray &words) {
    keywords = words;
    init_recognizer();
}

float SpeechRecognizer::get_sample_rate() const {
    return sample_rate;
}

void SpeechRecognizer::set_sample_rate(float rate) {
    sample_rate = rate;
    init_recognizer();
}