original_pwd=$(pwd)
current_script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $current_script_dir

qt_path="$current_script_dir/../Qt5.6/linux/centos7/bin/qmake-qt5"
cd src
make distclean
$qt_path ./asn1.pro -r -spec linux-clang CONFIG+=debug
make
$qt_path ./asn1.pro -r -spec linux-clang CONFIG+=release
make

cd $original_pwd 
