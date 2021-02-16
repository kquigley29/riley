# Riley
A C++ object detector library with Darknet

## Use
You will have to make sure that you have compiled Darknet with a shared object library.  
It is also important to set a `DARKNET` environment variable by executing
```bash
export DARKNET=<path to your Darknet directory>
```
or you can simply add this to your `.bashrc` file to avoid repetition.  
Similarly add the Darknet path to `LD_LIBRARY_PATH` environment variable by executing
```bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DARKNET
```
again you can put this in the `.bashrc` to avoid repetition.  

## Demo
If you want to see a demo, run the following from the riley directory after completing the first two steps from above:
```bash
mkdir build
cd build 
cmake .. -DDEMO=ON
make
./demo yolo
```
