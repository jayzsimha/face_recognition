You can setup opencv on your Mac using homebrew by following the link below. 
This is needed only if you would like to train the classifier on your laptop.
https://jjyap.wordpress.com/2014/05/24/installing-opencv-2-4-9-on-mac-osx-with-python-support/

Building code which uses opencv using cmake 
http://docs.opencv.org/2.4/doc/tutorials/introduction/linux_gcc_cmake/linux_gcc_cmake.html

Face recognition on Raspberry PI
	- Prepare face dataset
		- Take pictures using testPicture (built from $HOME/raspi/testStill.cpp)
		similar to the ones in images directory. Make sure the eyes are on the
		same horizontal line. It affects the classifier accuracy when the face is
		not aligned.
		- Extract faces from the pictures, using generate_traindata.pl. Make sure
		you have built extractfaces.cpp and there is a directory "faces" created
		in the directory which has the extractfaces binary.
		- create a csv file similar to the one in faces/image_list.csv. It is of
		the form
			<file_path>;<label>
	
	- Train a Face recognition classifier
		- Build train.cpp inside train_recogniser. To train the classifier just do
		./train <csv_file> <output directory>
		Ex: ./train image_list.csv .
		- This generates two classifiers
			- fisher_model.yaml
			- lbph_model.yaml
		- Copy these files to the classifiers directory.
		
	- Run the recogniser
		- Build classify.cpp inside recogniser and run it.
		- Make a note of the confidence values printed. Especially the difference 
		between a person in the data base and one who isn't. 
		- Choose a threshold on the confidence value to give you most correct
		classifications. 
		PS: The confidence values are actually distances (0-MAX_FLOAT). So, a 
		lower value means more stricter threshold, and higher value is more lenient.
		
		
		



