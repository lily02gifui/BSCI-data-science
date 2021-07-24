# Machine Learning Project1: Gaussian Naive Bayes Classification Algorithm

The Jupiter notebook file contains all the code and results needed/used in each question. 
This includes the codes to run the graphs/illustration used in the report.
All the libraries needed are placed at the top (1st python code block).
Please run the code consecutively in the same sequence as the codes were written in to avoid
any error.

## To run the code

### Preprocessing
To run the preprocessing function, input a file path to the csv file that you want preprocess as an argument
This will return a 2d array (list of rows) and class label 

The preprocessed data needs to be passed on to the train function to train the Gaussian Naive Bayes Model.

### Training
To run the train function to train the Gaussian Naive Bayes model, input the a 2D array containing the instances (a list of instances excluding the class), 
and a list of the class that correspond to each instance. This will calculate the prior probabilities of each class as well as the mean and standard deviation of each attribute for each class.
Hence, the function will return a dictionary that stores the prior probabilites for each class, and a dictionary that stores a list of the mean and a list of standard deviation of each attributes for each given class.

These resulting dictionary needs to be passed on to the predict function to predict the test instances.

### Testing 
To run the testing function, input the following 3 arguments:
"priors" which is a dictionary of priors (values) for each class (key),
"dist" which is a dictionary with a class as a key and a 2d array (a list of two lists:
a list of mean and a list std for each attributes) as a value
and "X_test" which is the 2d array of test data (list of rows from the test file)
This will return predicted class labels in list format with gaussian naive bayes

The predicted class labels needs to be passed on to the evaluate function in order to evaluate the predicting performance.

### Evaluate
To run the evaluate function, input a list of the class predicted by the model, and a list of the true class.
The function will evaluates the prediction performance by accessing how many correct instance is predicted, 
as well as calculating the precision, recall and f-score for each class. Thus, the function will return the proportion of correct prediction and a dictionary that stores the precision, recall,and f-score for each class so that the accuracy of the model is convey and if needed the precision, recall, and f-score of each class is also available for further understanding.

## Requirements

### installation
In the case where there is an error regarding the use of the function histplot( ) from the seaborn
library:
AttributeError: module ‘seaborn’ has no attribute ‘histplot’
Solution: please update seaborn to the latest version (0.11.1)

'''bash

pip install seaborn

'''

### usage

'''python

from collections import defaultdict
from math import sqrt
import numpy as np
import pandas as pd
import math
import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split

'''

## Authors
Aoi Fujii 
Sahasvichni Chou 
