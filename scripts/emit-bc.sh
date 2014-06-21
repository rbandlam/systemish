# Create LLVM (.ll) IR for a file
if [ "$#" -ne 1 ]; then
    echo "Illegal number of parameters"
	echo "Usage: ./emit-bc.sh <.cpp file>"
	exit
fi

clang -emit-llvm -c $@

