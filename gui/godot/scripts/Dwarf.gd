# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright 2018 Sacha Delanoue

extends KinematicBody2D

signal finished_moving

var modulate_color = Color(1, 1, 1, 1)
var moving = false
var _moving_to = Vector2()
var _dash = false

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

func move_to(to, dash, pushed):
	assert not moving
	assert not pushed or dash
	_moving_to = to
	_dash = dash
	moving = true
	var anim = "die" if pushed else ("slide" if dash else "walk")
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

func _process(delta):
	if moving:
		var diff = _moving_to - position
		if diff == Vector2():
			stop()
		else:
			var speed = (SPEED * 2.5 if _dash else SPEED) * global.speed_factor
			if diff.length() > speed * delta:
				position += (diff.normalized() * speed * delta)
			else:
				position = _moving_to
				stop()
