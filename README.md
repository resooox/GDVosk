# GDVosk
## A GDExtension for offline speech-recognition

This plugin was made for a project of mine - if you wish to use it for your own feel free to do so. HOWEVER, I compiled the files so that they'd work on my machine, you may need to rework anything from the SConstruct file to downloading different binaries from the Vosk GitHub repository. [Vosk Repository](https://github.com/alphacep/vosk-api)

## Usage

Go to [Vosk Model Downloads](https://alphacephei.com/vosk/models) and download any model you like, then import it to your Godot project. Create a SpeechRecognizer Node and assign a SpeechModel with a valid path to the folder that you downloaded. NOTE: Setting specific keywords for the recognizer only works for models have look-ahead support, pre-compiled HCLG graph models WILL NOT WORK!

Download the demo project to see how it works.