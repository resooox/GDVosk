extends SpeechRecognizer

const test_recording = preload("uid://c7dk3e886emfv")

func _ready() -> void:
	set_keywords(["dogs", "are", "sitting", "by", "the", "door"])
	send_waveform_pcm(test_recording.data)

func _on_result(text: String) -> void:
	print(text)

func _on_partial_result(text: String) -> void:
	print(text)
