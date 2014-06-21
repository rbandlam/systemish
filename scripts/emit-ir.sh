# Create LLVM (.ll) IR for a file
if [ "$#" -ne 1 ]; then
    echo "Illegal number of parameters"
	echo "Usage: ./emit-ir.sh <.cpp file>"
	exit
fi

clang -S -emit-llvm -c $@

