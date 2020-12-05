'''
Elements of Data Processing Project 2, Task 2a.
Written by Aoi Fujii in May 2020.

Reference to week9 workshop answer: K-NN  and Decision Tree Classification
https://canvas.lms.unimelb.edu.au/courses/12012/files/3547707/download?download_frd=1
'''

import pandas as pd
from sklearn import neighbors
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score
from sklearn import preprocessing
import numpy as np
from sklearn.impute import SimpleImputer
from sklearn.tree import DecisionTreeClassifier
#from decimal import Decimal


# predict the class feature life expectancy at birth(years) of the data

# Stage1: Split the dataset into a training set comprising 2/3 of the data and a test set comprisingthe remaining 1/3 
# using the traintestsplitfunction with a randomstate of 100.

# load in the data
world = pd.read_csv('world.csv', encoding = 'ISO-8859-1')
life = pd.read_csv('life.csv', encoding = 'ISO-8859-1')
merged = pd.merge(world, life, how='inner', on=['Country Code']).drop(['Country', 'Time'], axis = 1)

# get header
header = merged.columns.tolist()

# get feature name
for nonfeature in ['Country Name', 'Year', 'Country Code', 'Life expectancy at birth (years)']:
    header.remove(nonfeature)

# remove the '..' so that it can be converted to float
imp = SimpleImputer(missing_values='..', strategy='constant', fill_value=np.nan)
imp.fit(merged[header])
feature = imp.transform(merged[header])

# get just the features
# convert it to float
data = feature.astype(float)

# get just the class labels
classlabel=merged['Life expectancy at birth (years)']

# randomly select 2/3 of the instances to be training and the rest to be testing
X_train, X_test, y_train, y_test = train_test_split(data, classlabel, train_size=0.66, test_size=0.34, random_state=100)




#  Stage2: Perform the same imputation and scaling to the training set:
# For each feature, perform median imputation to impute missing values
impm = SimpleImputer(missing_values=np.nan, strategy='median')
impm.fit(X_train)

# get the median, round to 3dp
X_train_df = pd.DataFrame(data=X_train, columns=header)
median = X_train_df.median().round(3)

# imputation
X_train = impm.transform(X_train)


# Scale each feature by removing the mean and scaling to unit variance.
# (normalise the data to have 0 mean and unit variance using the library functions.) 
scaler = preprocessing.StandardScaler().fit(X_train)

# get the mean and variance used for scaling, round to 3dp
mean = pd.Series(scaler.mean_).round(3)
variance = pd.Series(scaler.var_).round(3)

# scale
X_train = scaler.transform(X_train)




# create csv describing the median, mean, variance for each feature
df = pd.DataFrame({'feature': median.index, 'median': median.values, 'mean': mean, 'variance': variance})
df.to_csv('task2a.csv', index=False)




# Stage3: Train the classifiers using the training set
### k-NN (k=5)
knn5 = neighbors.KNeighborsClassifier(n_neighbors=5)
knn5.fit(X_train, y_train)

### k-NN (k=10)
knn10 = neighbors.KNeighborsClassifier(n_neighbors=10)
knn10.fit(X_train, y_train)

### decision tree
dt = DecisionTreeClassifier(max_depth=4)#, random_state=1, criterion='entropy')
dt.fit(X_train, y_train)




# Stage 4:
# impute median and scale the test set
X_test = impm.transform(X_test)
X_test = scaler.transform(X_test)

# Test the classifiers by applying them to the test set.
# k-NN (k=5)
y_pred_k5 = knn5.predict(X_test)
# k-NN (k=10)
y_pred_k10 = knn10.predict(X_test)
# decision tree
y_pred_dt = dt.predict(X_test)

# accuracy
accuracy_k5 = accuracy_score(y_test, y_pred_k5)
accuracy_k10 = accuracy_score(y_test, y_pred_k10)
accuracy_dt = accuracy_score(y_test, y_pred_dt)

# take a float and convert it to a decimal, then round to 3 dp, change to percentage 
#percentage_k5 = round(Decimal(accuracy_k5), 3) * 100
#percentage_k10 = round(Decimal(accuracy_k10), 3) * 100
#percentage_dt = round(Decimal(accuracy_dt), 3) * 100




# print the classification accuracy
print(f'Accuracy of decision tree: {accuracy_dt * 100:.3f}%')
print(f'Accuracy of k-nn (k=5): {accuracy_k5 * 100: .3f}%')
print(f'Accuracy of k-nn (k=10): {accuracy_k10 * 100: .3f}%')



'''
# code for figure 1 used in a report
from matplotlib import pyplot as plt
import numpy
ks = range(1,30,1)
accu_list=[]
for k in ks:
    knn = neighbors.KNeighborsClassifier(n_neighbors=k)
    knn.fit(X_train, y_train) 
    y_pred=knn.predict(X_test)
    accu_list.append(accuracy_score(y_test, y_pred))
plt.plot(ks,accu_list)
plt.title("Figure 1. varying k versus the prediction accuracy for K-NN")
plt.xlabel("k value")
plt.ylabel("accuracy (%)")
plt.xlim([0, 30])
plt.savefig('figure1.png', bbox_inches='tight')
'''