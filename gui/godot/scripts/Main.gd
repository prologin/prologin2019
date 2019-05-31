extends Node

func _ready():
	if OS.has_feature('JavaScript'):
		#OS.set_window_maximized(true)
		get_tree().change_scene("res://scenes/Replay.tscn")
		return
	for arg in OS.get_cmdline_args():
		if arg.begins_with("-json="):
			get_tree().change_scene("res://scenes/Replay.tscn")
			return
		if arg.begins_with("-socket="):
			Global.spectator = true
			get_tree().change_scene("res://scenes/Replay.tscn")
			return
	get_tree().quit()