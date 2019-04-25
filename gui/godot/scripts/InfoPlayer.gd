# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright 2018 Sacha Delanoue

extends Node2D

class Player:
	var name = ""
	var score = 0
	var action_points = []
	var move_points = []

var players = [Player.new(), Player.new()]
var _turn = 0
var _type = 0
var _turn_slider = null
var _selected_dwarf = -1

func _ready():
	$Player1.set("custom_colors/font_color", Color(0, 0.5, 1, 1))
	$Player2.set("custom_colors/font_color", Color(1, 0.5, 0, 1))
	$Speed.text = "Vitesse : " + str(Global.speed_factor)
	$SpeedSlider.connect("value_changed", self, "_speed_slider")
	redraw()
	players[0].action_points.resize(Constants.NB_NAINS)
	players[1].action_points.resize(Constants.NB_NAINS)

func _redraw_player(id, label):
	label.text = players[id].name + "\nScore : " + str(players[id].score)

func redraw():
	_redraw_player(0, $Player1)
	_redraw_player(1, $Player2)
	_redraw_dwarf()
	$Turn.text = "Tour : " + str(_turn) + "\n" + ["Nains", "Bleu", "Rouge"][_type]

func set_turn(turn, type):
	_turn = turn
	_type = type
	for dwarf_id in range(Constants.NB_NAINS):
		players[0].action_points[dwarf_id] = 0
		players[1].action_points[dwarf_id] = 0
		players[0].move_points[dwarf_id] = 0
		players[1].move_points[dwarf_id] = 0
		if type != 0:
			players[type - 1].action_points[dwarf_id] = Constants.NB_POINTS_ACTION
			players[type - 1].move_points[dwarf_id] = Constants.NB_POINTS_DEPLACEMENT
	if _turn_slider:
		_turn_slider.value = turn
	redraw()

func set_tile(pos, cave, ores):
	#fixme
	$Tile.text = "l. " + str(pos.y) + ", c. " + str(pos.x)
	#if wall:
	#	$Tile.text += "\nMur"
	#elif alien:
	#	$Tile.text += "\nAlien de " + str(alien.points) + " points\ntour " + \
	#			str(alien.first_turn) + " (+" + str(alien.duration) + "), " + \
	#			str(alien.capture) + "/" + str(constants.NB_TOURS_CAPTURE)

func set_dwarf(dwarf):
	_selected_dwarf = dwarf
	_redraw_dwarf()

func _redraw_dwarf():
	if _selected_dwarf == -1:
		$Dwarf.text = ""
	else:
		var id = _selected_dwarf % Constants.NB_NAINS
		var player = (_selected_dwarf - id) / Constants.NB_NAINS
		var points_ac = players[player].action_points[id]
		var points_mv = players[player].move_points[id]
		$Dwarf.text = "Joueur " + str(player + 1) + "\nNain " + str(id) + "\nPA: " + str(points_ac) +"\nPM: " + str(points_mv)

func _speed_slider(value):
	var v = 1 << int(value)
	Global.speed_factor = v
	$Speed.text = "Vitesse : " + str(v)

func _process(delta):
	if Input.is_action_just_pressed("ui_up"):
		if $SpeedSlider.get_value() < $SpeedSlider.get_max():
			$SpeedSlider.set_value($SpeedSlider.get_value() + 1)
	elif Input.is_action_just_pressed("ui_down"):
		if $SpeedSlider.get_value() > $SpeedSlider.get_min():
			$SpeedSlider.set_value($SpeedSlider.get_value() - 1)

func add_turn_slider():
	_turn_slider = HSlider.new()
	_turn_slider.margin_left = 200
	_turn_slider.margin_right = 280
	_turn_slider.margin_top = 32
	_turn_slider.step = 1
	_turn_slider.min_value = 0
	_turn_slider.max_value = 99
	_turn_slider.set_focus_mode(Control.FOCUS_NONE)
	add_child(_turn_slider)
	return _turn_slider
