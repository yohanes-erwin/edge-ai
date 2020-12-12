// Author: Erwin Ouyang, aiotedge.tech
// Date  : 25 Jan 2020

#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11
#define NUM_OF_SAMPLES 80

DHT dht(DHTPIN, DHTTYPE);
// Training set
float x_train[NUM_OF_SAMPLES] = {
    36, 37, 27, 39, 46, 18,  0, 53, 10, 52, 24, 29, 25, 42, 25, 19, 54,
     0, 37, 23, 48,  8, 40,  3, 15, 28,  0, 26, 28, 25, 39,  9, 25, 30,
    27,  8,  9, 36, 47, 11, 74, 87, 57, 58, 92, 84, 58, 96, 91, 60, 75,
    87, 75, 69, 91, 77, 62, 69, 91, 93, 61, 57, 66, 66, 67, 61, 79, 96,
    64, 61, 88, 74, 65, 76, 80, 58, 81, 65, 85, 99};
float y_train[NUM_OF_SAMPLES] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
float theta_0 = 0.0;
float theta_1 = 0.0;

void setup()
{
  // Initialize serial port
  Serial.begin(115200);
  // Initialize DHT11
  dht.begin();

  printf("Rain Prediction using Simple Logistic Regression and DHT11:\n");

  // Training
  printf("Training ...\n");
  gradient_descent(x_train, y_train, NUM_OF_SAMPLES, 30000, 0.01);
  printf("Training completed: Theta 0 = %.3f, Theta 1 = %.3f\n", theta_0, theta_1);
}

void loop()
{
  float humidity = dht.readHumidity();
  float prediction = hypothesis(humidity);
  printf("Humidity = %.0f%, Probability of Rain = %.3f\n", humidity, prediction);
  delay(5000);
}

float sigmoid(float x)
{
  return 1/(1 + exp(-x));
}

float hypothesis(float x)
{
  return sigmoid(theta_0 + theta_1*x);
}

void gradient_descent(float x[], float y[], int m, int iter, float alpha)
{
  float h, e, sum_e, e_mul_x, sum_e_mul_x;
  
  for (int i = 0; i < iter; i++)
  {
    // Clear variable
    h = 0.0;
    e = 0.0;
    sum_e = 0.0;
    e_mul_x = 0.0;
    sum_e_mul_x = 0.0;
    
    for (int j = 0; j < m; j++)
    {
      // Calculate hypothesis
      h = hypothesis(x[j]);
      // Calculate error
      e = h - y[j];
      // Calculate sum of error
      sum_e = sum_e + e;      
      // Calculate error*x
      e_mul_x = e * x[j];
      // Calculate sum of error*x
      sum_e_mul_x = sum_e_mul_x + e_mul_x;
    }

    // Calculate d_J/d_theta_0
    float d_J_d_theta_0 = sum_e / m;  
    // Calculate d_J/d_theta_1
    float d_J_d_theta_1 = sum_e_mul_x / m;
    
    // Update theta 0
    theta_0 = theta_0 - alpha*d_J_d_theta_0;    
    // Update theta 1
    theta_1 = theta_1 - alpha*d_J_d_theta_1;
  }
}

