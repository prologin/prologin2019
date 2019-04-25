#!/usr/bin/env bash

pwd=`pwd`
build_dir="$pwd/build/"

rm -rf $build_dir
mkdir $build_dir
cd godot
godot --export "pck" $build_dir/prologin2019.pck

replay="$build_dir/prologin2019-replay"
echo -e "#!/usr/bin/env sh\n" > $replay
echo "test \$# -eq 0 && echo "Usage: \$0 dump.json" && exit 1" >> $replay
echo "godot --main-pack /opt/prologin2019.pck -json=\$1 \$2" >> $replay
chmod +x $replay

gui="$build_dir/prologin2019-gui"
echo -e "#!/usr/bin/env sh\n" > $gui
echo "test \$# -ne 2 && echo "Usage: \$0 port player_id" && exit 1" >> $gui
echo "godot --main-pack /opt/prologin2019.pck -socket=\$1 -id=\$2" >> $gui
chmod +x $gui

cd $build_dir
stechec2-generator player prologin2019 .
cp "$pwd/gui-stechec.cc" "$build_dir/cxx/prologin.cc"
make -C $build_dir/cxx

cp "$build_dir/prologin2019.pck" $pwd/../www/static/godot/

echo "The installer will install 2 files in /usr/bin, and 2 in /opt"
sudo install "$gui" "$replay" /usr/bin
sudo install "$build_dir/prologin2019.pck" /opt
#sudo install "$build_dir/cxx/champion.so" /opt/prologin2019-gui.so

echo "To uninstall:"
echo "sudo rm /usr/bin/prologin2019-{gui,replay} /opt/prologin2019{.pck,-gui.so}"

