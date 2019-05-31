extends Node2D


func set_color(colour):
	if colour == Constants.flag.DRAPEAU_VERT:
		$AnimatedSprite.play("green")
	elif colour == Constants.flag.DRAPEAU_ROUGE:
		$AnimatedSprite.play("red")
	elif colour == Constants.flag.DRAPEAU_BLEU:
		$AnimatedSprite.play("blue")