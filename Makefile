all:mkinfo
mkinfo:mkinfo.cpp
	g++ -o mkinfo mkinfo.cpp -lopencv_core -lopencv_objdetect -lopencv_highgui -lopencv_imgproc
