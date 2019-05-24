extends Node

func _ready():
	$SpeedSlider/Speed.text = "Vitesse: " + str(Global.speed_factor)
	$SpeedSlider.connect("value_changed", self, "_speed_slider")

func _speed_slider(value):
	var v = 1 << int(value)
	Global.speed_factor = v
	$SpeedSlider/Speed.text = "Vitesse: " + str(Global.speed_factor)
	
func _process(delta):
	if Input.is_action_just_pressed("ui_up"):
		if $SpeedSlider.get_value() < $SpeedSlider.get_max():
			$SpeedSlider.set_value($SpeedSlider.get_value() + 1)
	if Input.is_action_just_pressed("ui_down"):
		if $SpeedSlider.get_value() < $SpeedSlider.get_min():
			$SpeedSlider.set_value($SpeedSlider.get_value() - 1)

func init(players):
	$"Name 1".text = players[0].name
	$"Name 2".text = players[1].name
	$Turn.text = "0 / 100"
	$"Score 1".text = "0"
	$"Score 2".text = "0"
