#include "speech_model.hpp"
#include "vosk/vosk_api.h"
#include <godot_cpp/classes/project_settings.hpp>

using namespace godot;

SpeechModel::SpeechModel() {
    model = nullptr;
}

SpeechModel::~SpeechModel() {
    if (model != nullptr) {
        vosk_model_free(model);
        model = nullptr;
    }
}

void SpeechModel::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_model_path"), &SpeechModel::get_model_path);
    ClassDB::bind_method(D_METHOD("set_model_path", "path"), &SpeechModel::set_model_path);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "model_path", PropertyHint::PROPERTY_HINT_DIR), "set_model_path", "get_model_path");
}

String SpeechModel::get_model_path() const {
    return model_path;
}

void SpeechModel::set_model_path(const String &path) {
    model_path = path;

    if (model != nullptr) {
        vosk_model_free(model);
        model = nullptr;
    }

    model = vosk_model_new(ProjectSettings::get_singleton()->globalize_path(model_path).utf8().get_data());

    ERR_FAIL_NULL_MSG(model, "Failed to create Vosk model.");
}

VoskModel *SpeechModel::get_model() const {
    return model;
}