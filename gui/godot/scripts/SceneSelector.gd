# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright 2018 Sacha Delanoue

extends Node

func _ready():
	if OS.has_feature('JavaScript'):
		get_tree().change_scene("res://scenes/Replay.tscn")
		return
	for arg in OS.get_cmdline_args():
		if arg.begins_with("-json="):
			get_tree().change_scene("res://scenes/Replay.tscn")
			return
		if arg.begins_with("-socket="):
			get_tree().change_scene("res://scenes/Spectator.tscn")
			return
	get_tree().quit()
