# HandWritingDetection
The primary objective of this project is to implement handwriting text recognition on the Arduino platform using a deep neural network. However, certain components of the network, such as the Convolutional Neural Network, demand significant computational power, posing a challenge for the hardware's capabilities. To address this limitation, we initially conducted pre-training using the IAM dataset on more powerful computers. Subsequently, we transitioned to the Arduino for the second phase of training, focusing on the dense layers. To enhance the model's performance, we incorporated L2-regularization, dropout techniques, and image pre-processing. The efficacy of the trained model was evaluated using a set of forty test images, resulting in a final accuracy of 0.77.
![alt text](https://github.com/batudrsnn/HandWritingDetection/blob/main/test_results.png?raw=true)

Check the website https://youtu.be/vcx4aTHGlco for demonstration.
