# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright 2019 Martin Huvelle

extends KinematicBody2D

signal finished_moving

var modulate_color = Color(1, 1, 1, 1)
var moving = true
var mining = false
var _moving_to = Vector2()
var _mining_to = Vector2()

const SPEED = 100

func set_team(blue):
	if blue:
		modulate_color = Color(0.5, 0.75, 1, 1)
	else:
		modulate_color = Color(1, 0.75, 0.5, 1)
	set_modulate(modulate_color)

func focus():
	set_modulate(modulate_color.lightened(0.7))

func unfocus():
	set_modulate(modulate_color)

func move_to(to):
	assert not moving
	_moving_to = to
	moving = true
	var anim = null
	if (to == "EAST" or to == "WEST"):
		anim = "move"
	#else if $TileMap.getTile():
	else:
		anim = "climb"
	$AnimatedSprite.play(anim)
	var dx = to.x - position.x
	if dx > 0:
		$AnimatedSprite.flip_h = false
	elif dx < 0:
		$AnimatedSprite.flip_h = true

func mine_to(to):
	assert not mining
	_mining_to = to
	mining = true
	var anim = "mine"
	$AnimatedSprite.play(anim)
	var dx = to.x - position.x
	if dx > 0:
		$AnimatedSprite.flip_h = false
	elif dx < 0:
		$AnimatedSprite.flip_h = true
	
func stop():
	$AnimatedSprite.play("idle")
	if moving:
		emit_signal("finished_moving")
	moving = false
	mining = false

func _process(delta):
	if moving:
		var diff = _moving_to - position
		if diff == Vector2():
			stop()
		else:
			var speed = SPEED * Global.speed_factor
			if diff.length() > speed * delta:
				position += (diff.normalized() * speed * delta)
			else:
				position = _moving_to
				stop()
