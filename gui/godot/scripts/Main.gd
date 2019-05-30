extends Node

func _ready():
	if OS.has_feature('JavaScript'):
		get_tree().change_scene("res://scenes/Replay.tscn")
		return
	for arg in OS.get_cmdline_args():
		if "-names=" in arg:
			Global.champion_names = arg.replace("-names=","")
			print(Global.champion_names)
		if arg.begins_with("-json="):
			get_tree().change_scene("res://scenes/Replay.tscn")
			return
		if arg.begins_with("-socket="):
			get_tree().change_scene("res://scenes/Specator.tscn")
			return
	get_tree().quit()