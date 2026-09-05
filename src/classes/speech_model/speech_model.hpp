#pragma once

#include "vosk/vosk_api.h"
#include <godot_cpp/classes/resource.hpp>

namespace godot {

    class SpeechModel : public Resource {
        GDCLASS(SpeechModel, Resource);

        private:
            VoskModel *model = nullptr;
            String model_path;

        protected:
            static void _bind_methods();
        
        public:
            SpeechModel();
            ~SpeechModel();

            String get_model_path() const;
            void set_model_path(const String &path);

            VoskModel *get_model() const;
    };

}
