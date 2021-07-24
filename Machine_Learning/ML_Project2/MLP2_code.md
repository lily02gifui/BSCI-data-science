# Machine Learning Project2: Multiclass classification to predict recipe features

The Jupiter notebook file contains the code and results needed/used for the report. 
This includes the codes to run the graphs/illustration used in the report.
All the libraries needed are placed at the top (1st python code block).
Please run the code consecutively in the same sequence as the codes were written in to avoid any error.

## To run the code

### load provided data
load the provided data for this assignment to your local envitonment and run the code to process data.
read in the attributes by running "load attribute" section.

### Feature selection
run the code segument for each feature selection section to select feature for diffenrt data set and feature engineeriung methods.

### evaluation
evaluate the accuracy and analyse the error with visualisation by running the code seguments.

You do not have to input any variables specifically to run these codes.
Plese follow each code segument and run sequencially.

## Requirements

### usage

'''python
import pandas as pd
import numpy as np
import pickle
import scipy
from sklearn.feature_selection import SelectKBest, chi2,  mutual_info_classif
from sklearn.feature_extraction.text import TfidfTransformer
from sklearn.model_selection import train_test_split
from sklearn.decomposition import PCA
from sklearn.svm import SVC
from sklearn.feature_selection import SequentialFeatureSelector
from sklearn.model_selection import GridSearchCV
from sklearn.metrics import classification_report
from sklearn.naive_bayes import GaussianNB
  
from sklearn import svm
from sklearn.tree import DecisionTreeClassifier
from sklearn.dummy import DummyClassifier
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score
from sklearn.metrics import confusion_matrix
import matplotlib.pyplot as plt
import seaborn as sns
import time
from sklearn.metrics import mean_squared_error
from collections import defaultdict as dd
from sklearn.model_selection import StratifiedShuffleSplit
'''

## Author
Aoi Fujii 
