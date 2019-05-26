extends Node

var is_end = 0

func _ready():
	$SpeedSlider/Speed.text = "Vitesse: " + str(Global.speed_factor)
	$SpeedSlider.connect("value_changed", self, "_speed_slider")

func _speed_slider(value):
	var v = 1 << int(value)
	Global.speed_factor = v
	$SpeedSlider/Speed.text = "Vitesse: " + str(Global.speed_factor)
	
func _end():
	var name_winner = ""
	var is_end = 1
	if $"Score 1" == $"Score 2":
		$End.text = "Egalité\n\n Appuyer sur Esc pour quitter"
		return
	if $"Score 1" > $"Score 2":
		name_winner = $"Name 1".text
	else:
		name_winner = $"Name 2".text
	$End.text = name_winner + " est le plus riche!\n\n Appuyer sur Esc pour quitter"

func _process(delta):
	if is_end:
		if Input.is_action_just_pressed("ui_escape"):
			get_tree().quit()
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
