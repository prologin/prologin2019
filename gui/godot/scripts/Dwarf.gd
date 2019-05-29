# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright 2019 Martin Huvelle

extends KinematicBody2D

signal finished_moving

var moving = false
var mining = false
var stick = false
var roping = false
var dying = false
var pulling = false
var dancing = false
var _moving_to = Vector2()
var _mining_to = Vector2()
var external_pos = Vector2()

const SPEED = 100

func set_sticky(is_stick):
	stick = is_stick

func set_external_position(pos, map):
	external_pos = pos
	position = map.world_position(pos)

func move_to(external_to, map, tile):
	#assert not moving
	external_pos = external_to
	var to = map.world_position(external_to)
	_moving_to = to
	moving = true
	var anim = null
	var dx = to.x - position.x
	if stick and dx == 0:
		if dx == 0:
			if tile == 4:
				anim = "climb_rope"
			else:
				anim = "climb_v"
		else:
			anim = "climb_h"
	else:
		anim = "move"
	$AnimatedSprite.set_speed_scale(Global.speed_factor)
	$AnimatedSprite.play(anim)

	if dx > 0:
		$AnimatedSprite.flip_h = false
	elif dx < 0:
		$AnimatedSprite.flip_h = true

func mine_to(external_to, map):
	#assert not mining
	var to = map.world_position(external_to)
	_mining_to = to
	mining = true
	var anim = "mine"
	$AnimatedSprite.set_speed_scale(Global.speed_factor)
	$AnimatedSprite.play(anim)
	var dx = to.x - position.x
	if dx > 0:
		$AnimatedSprite.flip_h = false
	elif dx < 0:
		$AnimatedSprite.flip_h = true
	
func set_rope_to():
	roping = true

func pull():
	assert not pulling
	pulling = true
	$AnimatedSprite.set_speed_scale(Global.speed_factor)
	$AnimatedSprite.play("pull")

func dance():
	assert not dancing
	dancing = true
	$AnimatedSprite.set_speed_scale(Global.speed_factor)
	$AnimatedSprite.play("dance")

func die():
	assert not dying
	dying = true
	$AnimatedSprite.set_speed_scale(Global.speed_factor)
	$AnimatedSprite.play("die")

func grab_to():
	assert not mining
	stick = true
	$AnimatedSprite.set_speed_scale(Global.speed_factor)
	$AnimatedSprite.play("grab")
	
func pull_to():
	assert not mining
	$AnimatedSprite.set_speed_scale(Global.speed_factor)
	$AnimatedSprite.play("climb")
	
func stop():
	moving = false
	mining = false
	dying = false
	pulling = false
	emit_signal("finished_moving")
	$AnimatedSprite.set_speed_scale(Global.speed_factor)
	$AnimatedSprite.play("idle")

func animation_finished():
	if mining or (stick and not moving) or roping or dying or pulling:
		stop()

func _ready():
	$AnimatedSprite.connect("animation_finished", self, "animation_finished")

func _process(delta):
	if moving:
		var diff = _moving_to - position
		if diff == Vector2(0, 0):
			stop()
		else:
			var speed = SPEED * Global.speed_factor
			if diff.length() > speed * delta:
				position += (diff.normalized() * speed * delta)
			else:
				position = _moving_to
				stop()