#! /bin/bash

height=30
width=30
name="gui.png"

background=('granite/granite_dark.png' 'granite/granite_light.png')
back_len=${#background[@]}

characters=(mine/** move/**)
char_len=${#characters[@]}

gen_img()
{
    img=${background[$(($RANDOM % $back_len))]}
    rnd=$(($RANDOM % ($char_len * 2)))
    if [ $rnd -ge $char_len ]; then
        cp $img $img_name
    else
        convert -composite $img ${characters[$rnd]} -gravity center $img_name
    fi
}

gen_line()
{
    line=""
    for nb in $(seq $height); do
        img_name="img_$nb.png"
        gen_img
        line="$line $img_name"
    done
    convert $line +append $line_name 2> /dev/null
    rm -f $line
}

gen_gui()
{
    lines=""
    for nb in $(seq $height); do
        line_name="line_$nb.png"
        echo "Generating $line_name"
        gen_line
        lines="$lines $line_name"
    done
    convert $lines -append "$name" 2> /dev/null
    rm $lines
}

gen_gui
