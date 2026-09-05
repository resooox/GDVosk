#pragma once

#include <godot_cpp/classes/node.hpp>

#include "vosk/vosk_api.h"
#include "classes/speech_model/speech_model.hpp"


namespace godot {

    class SpeechRecognizer : public Node {
        GDCLASS(SpeechRecognizer, Node);
    
        private:
            Ref<SpeechModel> speech_model;
            
            VoskRecognizer *recognizer;

            String last_partial;

            float sample_rate = 16000.0f;
            PackedStringArray keywords;

            void init_recognizer();
            void stop_recognition();

        protected:
            static void _bind_methods();
            void _notification(int p_what);

        public:
            SpeechRecognizer();
            ~SpeechRecognizer();

            void send_waveform_pcm(const PackedByteArray &data);

            Ref<SpeechModel> get_language_model() const;
            void set_language_model(Ref<SpeechModel> model);

            PackedStringArray get_keywords() const;
            void set_keywords(const PackedStringArray &words);

            float get_sample_rate() const;
            void set_sample_rate(float rate);
    };

}