# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright 2019 Martin Huvelle

extends KinematicBody2D

signal finished_moving

var modulate_color = Color(1, 1, 1, 1)
var moving = false
var mining = false
var stick = false
var roping = false
var _moving_to = Vector2()
var _mining_to = Vector2()
var external_pos = Vector2()

const SPEED = 100

func set_team(player_id):
	if player_id == 0:
		modulate_color = Color(0.5, 0.75, 1, 1)
	else:
		modulate_color = Color(1, 0.75, 0.5, 1)
	set_modulate(modulate_color)

func set_external_position(pos, map):
	external_pos = pos
	position = map.world_position(pos)

func focus():
	set_modulate(modulate_color.lightened(0.7))

func unfocus():
	set_modulate(modulate_color)

func move_to(external_to, map):
	assert not moving
	external_pos = external_to
	var to = map.world_position(external_to)
	_moving_to = to
	moving = true
	var anim = null
	var dx = to.x - position.x
	if (stick and dx == 0):
		if ($TileMap.get_tile(position.x, position.y) == 4):
			anim = "climb"
		else:
			anim = "grab"
	else:
		anim = "move"
		stick = false
	$AnimatedSprite.set_speed_scale(Global.speed_factor)
	$AnimatedSprite.play(anim)

	if dx > 0:
		$AnimatedSprite.flip_h = false
	elif dx < 0:
		$AnimatedSprite.flip_h = true

func mine_to(external_to, map):
	assert not mining
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
	
func set_rope_to(external_to, map):
	assert not mining
	assert not moving
	assert not roping
	roping = true
	
func grab_to(map):
	assert not mining
	stick = true
	$AnimatedSprite.set_speed_scale(Global.speed_factor)
	$AnimatedSprite.play("grab")
	
func pull_to():
	assert not mining
	$AnimatedSprite.set_speed_scale(Global.speed_factor)
	$AnimatedSprite.play("climb")
	
func stop():
	$AnimatedSprite.set_speed_scale(Global.speed_factor)
	$AnimatedSprite.play("idle")
	emit_signal("finished_moving")
	moving = false
	mining = false

func animation_finished():
	if mining or (stick and not moving) or roping:
		stop()

func _ready():
	$AnimatedSprite.connect("animation_finished", self, "animation_finished")

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