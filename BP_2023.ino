#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<TinyMLShield.h>
#include <iostream>
#include <sstream>
#include <vector>

// NN parameters, set these yourself! 
#define LEARNING_RATE 0.08    // The learning rate used to train your network
#define EPOCH 15             // The maximum number of epochs 
#define DATA_TYPE_FlOAT      // The data type used: Set this to DATA_TYPE_DOUBLE for higher precision. However, it is better to keep this Float if you want to submit the result via BT

extern const int first_layer_input_cnt;
extern const int classes_cnt;
bool commandRecv = false;

// You define your network in NN_def
// Right now, the network consists of three layers: 
// 1. An input layer with the size of your input as defined in the variable first_layer_input_cnt in cnn_data.h 
// 2. A hidden layer with 20 nodes
// 3. An output layer with as many classes as you defined in the variable classes_cnt in cnn_data.h 
static const unsigned int NN_def[] = {first_layer_input_cnt, 30, classes_cnt};

#include "data.h"       // The data, labels and the sizes of all objects are stored here 
#include "NN_functions.h"   // All NN functions are stored here 

int iter_cnt = 0;           // This keeps track of the number of epochs you've trained on the Arduino
#define DEBUG 0             // This prints the weights of your network in case you want to do debugging (set to 1 if you want to see that)


// This function contains your training loop 
void do_training() {

  // Print the weights if you want to debug 
#if DEBUG      
  Serial.println("Now Training");
  PRINT_WEIGHTS();
#endif

  // Print the epoch number 
  Serial.print("Epoch count (training count): ");
  Serial.print(++iter_cnt);
  Serial.println();

  // reordering the index for more randomness and faster learning
  shuffleIndx();
  
  // starting forward + Backward propagation
  for (int j = 0;j < numTrainData;j++) {
    generateTrainVectors(j);  
    forwardProp();
    backwardProp();
  }

  Serial.println("Accuracy after local training:");
  printAccuracy();

}


void setup() {
  // put your setup code here, to run once:
  
  // Initialize random seed 
  srand(0); 
  
  Serial.begin(9600); 
  delay(5000);
  while (!Serial); 

  // Initialize the TinyML Shield 
  initializeShield();

  // Calculate how many weights and biases we're training on the device. 
  int weights_bias_cnt = calcTotalWeightsBias(); 

  Serial.print("The total number of weights and bias used for on-device training on Arduino: ");
  Serial.println(weights_bias_cnt);

  // Allocate common weight vector, and pass to setupNN, setupBLE
  DATA_TYPE* WeightBiasPtr = (DATA_TYPE*) calloc(weights_bias_cnt, sizeof(DATA_TYPE));

  setupNN(WeightBiasPtr);  // CREATES THE NETWORK BASED ON NN_def[]
  Serial.print("The accuracy before training");
  printAccuracy();
  
  Serial.println("Use the on-shield button to start and stop the loop code ");
  
}

// void convertString(char string) {
//     std::string input = string;

//     // Remove the curly braces from the string
//     input = input.substr(1, input.length() - 2);

//     // Use stringstream to split the string into individual elements
//     std::stringstream ss(input);
//     std::string token;
//     std::vector<int> elements;

//     while (getline(ss, token, ',')) {
//         // Convert each element from string to integer
//         int element = std::stoi(token);
//         elements.push_back(element);
//     }

//     // Convert the vector to a real array
//     int size = elements.size();
//     int *realArray = new int[size];

//     for (int i = 0; i < size; ++i) {
//         realArray[i] = elements[i];
//     }

//     // Now you can use the realArray as needed

//     // // Print the elements of the array as a test
//     // std::cout << "Converted array: ";
//     // for (int i = 0; i < size; ++i) {
//     //     std::cout << realArray[i] << " ";
//     // }
//     // std::cout << std::endl;

//     // // Don't forget to free the memory allocated for the array
//     // delete[] realArray;

//     return realArray;
// }

void loop() {
  // put your main code here, to run repeatedly:
  String command;
  //std::string command;
  // see if the button is pressed and turn off or on recording accordingly
  bool clicked = readShieldButton();
  
  if (iter_cnt <= EPOCH){
    
    Serial.println("Training...");
    do_training(); // Local training 
    
  } else {
    while (Serial.available()) {
      char c = Serial.read();
      if ((c != '\n') && (c != '\r')) {
        command.concat(c);
        //command = command + c;
      } 
      else if (c == '\r') {
        commandRecv = true;
        //command.toLowerCase();
    }

    if (commandRecv) {

      std::string input = command.c_str();

      // Remove the curly braces from the string
      input = input.substr(1, input.length() - 2);

      // Use stringstream to split the string into individual elements
      std::stringstream ss(input);
      std::string token;
      std::vector<double> elements;

      while (getline(ss, token, ',')) {
          // Convert each element from string to integer
          int element = std::stod(token);
          elements.push_back(element);
      }

      // Convert the vector to a real array
      int size = elements.size();
      double *realArray = new double[size];

      for (int i = 0; i < size; ++i) {
          realArray[i] = elements[i];
      }

      predict(realArray);

      delete[] realArray;
    }
  }
  }

}
