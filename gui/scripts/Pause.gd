extends Popup

# Declare member variables here. Examples:
# var a = 2
# var b = "text"

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	print("I am alive")
	if $Unpause.pressed:
		get_tree().paused = false
		self.hide()
	if $Quit.pressed:
		get_tree().quit()
