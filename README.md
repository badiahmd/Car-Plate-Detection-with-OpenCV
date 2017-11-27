# Car-Plate-Detection-with-OpenCV
This project is created to implement a License Plate Detection whereby to detect the car plate with image processing technique to differentiate between the surrounding (noises) and real car plates. The project is developed with C++ and OpenCV Library. Following are the summarized processes involved :<br />
•	Pre-processing the image of the captured car image.<br />
•	Convert the input image into the Grey Scale image.<br />
•	Enhance the input image and making it more suitable for the processing steps.<br />
•	Find the edge of the image by apply (-1 , 0 , +1) Filter for 3 x 3 rows and columns vertically<br />
•	Apply binary morphology : dilation<br />
•	Find the interconnected pixels or contours ,to find the plate<br />
