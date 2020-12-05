'''
Elements of Data Processing Project 2, Task 2b.
Written by Aoi Fujii in May 2020.

references
(line 159) https://towardsdatascience.com/pca-using-python-scikit-learn-e653f8989e60
(line 188) https://scikit-learn.org/stable/modules/generated/sklearn.preprocessing.PolynomialFeatures.html
(line 396) https://towardsdatascience.com/k-means-clustering-algorithm-applications-evaluation-methods-and-drawbacks-aa03e644b48a
(line 423) https://towardsdatascience.com/feature-selection-techniques-in-machine-learning-with-python-f24e7da3f36e
'''

import pandas as pd
from sklearn import neighbors
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score
from sklearn import preprocessing
import numpy as np
from sklearn.impute import SimpleImputer
#from decimal import Decimal
#from sklearn.tree import DecisionTreeClassifier
from sklearn.decomposition import PCA
from sklearn.feature_selection import SelectKBest, chi2
from sklearn.preprocessing import MinMaxScaler
from sklearn.cluster import KMeans
import matplotlib.pyplot as plt
import seaborn as sns


# create a training set and test set from world.csv and life.csv
def merged_world_life():
    # load in the data
    world = pd.read_csv('world.csv',encoding = 'ISO-8859-1')
    life = pd.read_csv('life.csv',encoding = 'ISO-8859-1')
    merged = pd.merge(world, life, how='inner', on=['Country Code']).drop(['Country', 'Time'], axis = 1)

    # get header
    header = merged.columns.tolist()

    # get feature name
    for nonfeature in ['Country Name', 'Year', 'Country Code', 'Life expectancy at birth (years)']:
        header.remove(nonfeature)

    #for i in merged['Adjusted savings: particulate emission damage (% of GNI) [NY.ADJ.DPEM.GN.ZS]']:
    #    print(i, type(i))

    # remove the '..' so that it can be converted to float
    imp = SimpleImputer(missing_values='..', strategy='constant', fill_value=np.nan)
    imp.fit(merged[header])
    feature = imp.transform(merged[header])

    # get the features
    data = feature.astype(float)
    
    #data_df = pd.DataFrame(data, columns=header)

    # get the class labels
    classlabel=merged['Life expectancy at birth (years)']
    
    return data, header, classlabel


data, header, classlabel = merged_world_life()

# randomly select 2/3 of the instances to be training and the rest to be testing
X_train, X_test, y_train, y_test = train_test_split(data, classlabel, train_size=0.66, test_size=0.34, random_state=1)




'''
The first four features from the original dataset are selected 
as a sample of the original 20 features. Then 5-NN classification is performed.

first4 = ['Access to electricity, rural (% of rural population) [EG.ELC.ACCS.RU.ZS]', 
'Adjusted savings: particulate emission damage (% of GNI) [NY.ADJ.DPEM.GN.ZS]', 
'Birth rate, crude (per 1,000 people) [SP.DYN.CBRT.IN]', 
'Cause of death, by communicable diseases and maternal, prenatal and nutrition conditions (% of total) [SH.DTH.COMM.ZS]']
'''

# take first 4 features
X_train4 = X_train[:,:4]
X_test4 = X_test[:,:4]

# Perform the same imputation and scaling to the training set:
# For each feature, perform median imputation to impute missing values
imp4 = SimpleImputer(missing_values=np.nan, strategy='median')
imp4.fit(X_train4)

# get the median, round to 3dp
#X_train4_df = pd.DataFrame(data=X_train4, columns=first4)
#median4 = X_train4_df.median().round(3)

# imputation
X_train4 = imp4.transform(X_train4)

# scale each feature by removing the mean and scaling to unit variance.
scaler4 = preprocessing.StandardScaler().fit(X_train4)

# get the mean and variance used for scaling, round to 3dp
#mean4 = pd.Series(scaler4.mean_).round(3)
#variance4 = pd.Series(scaler4.var_).round(3)

# scale
X_train4 = scaler4.transform(X_train4)

# Train the classifiers using the training set
knn4 = neighbors.KNeighborsClassifier(n_neighbors=5)
knn4.fit(X_train4, y_train)

# impute median and scale the test set
X_test4 = imp4.transform(X_test4)
X_test4 = scaler4.transform(X_test4)

# Test the classifiers by applying them to the test set.
# k-NN (k=5)
y_pred4 = knn4.predict(X_test4)

# accuracy
accuracy4 = accuracy_score(y_test, y_pred4)*100




# Perform the same imputation and scaling to the training set:
# For each feature, perform median imputation to impute missing values
imp = SimpleImputer(missing_values=np.nan, strategy='median')
imp.fit(X_train)

# get the median, round to 3dp
#X_train_df = pd.DataFrame(data=X_train, columns=header)
#median = X_train_df.median().round(3)

# imputation
X_train = imp.transform(X_train)
X_test = imp.transform(X_test)




'''
Feature engineering and selection via PCA are implemented by taking the first four principal
components. Only these four features are used to perform 5-NN classification.
'''
# scale each feature by removing the mean and scaling to unit variance.
scalerpca = preprocessing.StandardScaler().fit(X_train)

# get the mean and variance used for scaling, round to 3dp
#mean = pd.Series(scaler.mean_).round(3)
#variance = pd.Series(scaler.var_).round(3)

# scale
X_trainpca = scalerpca.transform(X_train)
X_test_pca = scalerpca.transform(X_test)

normalscalerpca = MinMaxScaler().fit(X_trainpca)
X_trainpca = normalscalerpca.transform(X_trainpca)
X_test_pca = normalscalerpca.transform(X_test_pca)

# pca by taking first four principal components
pca = PCA(n_components=4).fit(X_trainpca)
X_train_pca = pca.transform(X_trainpca)
X_test_pca = pca.transform(X_test_pca)

#X_train_df = pd.DataFrame(X_train, columns = header)

# perform 5-NN classification
# Train the classifiers using the training set
knn_pca = neighbors.KNeighborsClassifier(n_neighbors=5)
knn_pca.fit(X_train_pca, y_train)

# Test the classifiers by applying them to the test set.
# k-NN (k=5)
y_pred_pca = knn_pca.predict(X_test_pca)

# accuracy
accuracy_pca = accuracy_score(y_test, y_pred_pca)*100




'''
Feature engineering is implemented using interaction term pairs and clustering labels. 
A dataset with 211 features are produced (20 original features, 190 features
generated by interaction term pairs and 1 feature generated by clustering).  
Four features are selected from this dataset and 5-NN classification are performed.
'''

# feature engineering interaction term pair
poly = preprocessing.PolynomialFeatures(interaction_only=True, include_bias=False)
poly.fit(X_train)

# 210 generated by interaction term pairs
X_train_f = poly.transform(X_train)
X_test_f = poly.transform(X_test)
P1 = poly.get_feature_names(input_features=header)

# scale
scalereng = preprocessing.StandardScaler().fit(X_train_f)
X_train_f = scalereng.transform(X_train_f)
X_test_f = scalereng.transform(X_test_f)

normaleng =  MinMaxScaler().fit(X_train_f)
X_train_f = normaleng.transform(X_train_f)
X_test_f = normaleng.transform(X_test_f)



# Scale for cluster label
scalerc = preprocessing.StandardScaler().fit(X_train)

# get the mean and variance used for scaling, round to 3dp
#mean = pd.Series(scaler.mean_).round(3)
#variance = pd.Series(scaler.var_).round(3)

# scale
X_traineng = scalerc.transform(X_train)
X_test_eng = scalerc.transform(X_test)

normalscaler = MinMaxScaler().fit(X_traineng)
X_traineng = normalscaler.transform(X_traineng)
X_test_eng = normalscaler.transform(X_test_eng)

# feature engineering k means
#from sklearn.cluster import KMeans
kmean = KMeans(n_clusters=3).fit(X_traineng)
CL = kmean.cluster_centers_
f_kmean = kmean.labels_
f_kmean_test = kmean.predict(X_test_eng)

# 1 feature generated by kmeans clustering
f2d_kmean = np.reshape(f_kmean, (-1, len(f_kmean)))



# all the generted features 211 in total
generated_f = np.concatenate((X_train_f, f2d_kmean.T), axis=1)



# feature selelction using test data only
bestf = SelectKBest(chi2, k=4).fit(generated_f, y_train)
selected_f = bestf.transform(generated_f)
#print(len(selected_f))

#list of names for each feature
index_selected = bestf.get_support(indices=True)
feature_l = poly.get_feature_names(input_features=header)
feature_l.append('kmeans')


selected_f_names = []
for i in index_selected:
    selected_f_names.append(feature_l[i])
#print(selected_f_names)

    
    

# perform 5-NN classification

# Train the classifiers using the training set
knn_eng = neighbors.KNeighborsClassifier(n_neighbors=5)
knn_eng.fit(selected_f, y_train)




# 1 feature generated by kmeans clustering
f2d_kmean_test = np.reshape(f_kmean_test, (-1, len(f_kmean_test)))

# all the generted features 211 in total
generated_f_test = np.concatenate((X_test_f, f2d_kmean_test.T), axis=1)

# feature selelction using test data only
selected_f_test = bestf.transform(generated_f_test)

# Test the classifiers by applying them to the test set.
# k-NN (k=5)
y_pred_eng = knn_eng.predict(selected_f_test)

# accuracy
accuracy_eng = accuracy_score(y_test, y_pred_eng)*100




# accuracy of each method
print(f'Accuracy of feature engineering: {accuracy_eng: .3f}%')
print(f'Accuracy of PCA: {accuracy_pca: .3f}%')
print(f'Accuracy of first four features: {accuracy4: .3f}%')


print("----------")
print("### NOTE TO THE MARKER ###")
print("Please run the code for 5 png files(figure2-6) individually.")
print("Running all the code at the same time may result in producing incorrect images.")
print("This issue was reported on the discussion forum.")
print("All the copies of these images are included in task2c.pdf.")


# output

print("----------")
# interaction term pair
print(f'Total number of features (original and interacton term pair): {len(P1)}')
print(f'210 feature names (20 original, 190 by interacton term pair): {P1}')

print("----------")
#clustering
print(f'feature engineered from clustering (training set): {f_kmean}')
print(f'feature engineered from clustering (test set): {f_kmean_test}')


# heat map for the generated feature
# refer to workshop week 8
import seaborn as sns
import numpy as np
import math,random
from scipy.spatial.distance import pdist, squareform
import pandas as pd
import matplotlib.pyplot as plt

def VAT(R):
    """
    VAT algorithm adapted from matlab version:
    http://www.ece.mtu.edu/~thavens/code/VAT.m

    Args:
        R (n*n double): Dissimilarity data input
        R (n*D double): vector input (R is converted to sq. Euclidean distance)
    Returns:
        RV (n*n double): VAT-reordered dissimilarity data
        C (n int): Connection indexes of MST in [0,n)
        I (n int): Reordered indexes of R, the input data in [0,n)
    """
        
    R = np.array(R)
    N, M = R.shape
    if N != M:
        R = squareform(pdist(R))
        
    J = list(range(0, N))
    
    y = np.max(R, axis=0)
    i = np.argmax(R, axis=0)
    j = np.argmax(y)
    y = np.max(y)
    
    I = i[j]
    del J[I]

    y = np.min(R[I,J], axis=0)
    j = np.argmin(R[I,J], axis=0)
    
    I = [I, J[j]]
    J = [e for e in J if e != J[j]]
    
    C = [1,1]
    for r in range(2, N-1):   
        y = np.min(R[I,:][:,J], axis=0)
        i = np.argmin(R[I,:][:,J], axis=0)
        j = np.argmin(y)        
        y = np.min(y)      
        I.extend([J[j]])
        J = [e for e in J if e != J[j]]
        C.extend([i[j]])
    
    y = np.min(R[I,:][:,J], axis=0)
    i = np.argmin(R[I,:][:,J], axis=0)
    
    I.extend(J)
    C.extend(i)
    
    RI = list(range(N))
    for idx, val in enumerate(I):
        RI[val] = idx

    RV = R[I,:][:,I]
    
    return RV.tolist(), C, I

# get data
check=generated_f[:, index_selected] #retain a copy with only these columns

# Apply VAT Algorithm to Iris dataset and visualise using heatmap
RV, C, I = VAT(check)
x=sns.heatmap(RV,cmap='viridis',xticklabels=False,yticklabels=False)
x.set(xlabel='featuress', ylabel='features')
plt.title("Figure 2. VAT for the selected 4 features after feature engineering")
plt.savefig('figure2.png', bbox_inches='tight')


print(f'number of cluster centres: {len(CL)}')


# Run the Kmeans algorithm and get the index of data points clusters
sse = []
list_k = list(range(1, 10))

for k in list_k:
    km = KMeans(n_clusters=k)
    km.fit(X_train)
    sse.append(km.inertia_)

# Plot sse against k
plt.figure(figsize=(6, 6))
plt.plot(list_k, sse, '-o')
plt.xlabel('Number of clusters "k"')
plt.ylabel('Sum of squared distance');
plt.title("        Figure 3. Kmeans for differnt number of clusters")
plt.savefig('figure3.png', bbox_inches='tight')




print("----------")
# feature selection
# apply select  best to get top 4 feature

print(f'features used for this feature engineering method: {selected_f_names}')


# apply select  best to get top 4 feature
best = SelectKBest(chi2, k=4)
fs = best.fit(generated_f[:,:],y_train)
dfscores = pd.DataFrame(fs.scores_)
dfcolumns = pd.DataFrame(feature_l)
# concat two dataframes for better visualization 
featureScores = pd.concat([dfcolumns,dfscores],axis=1)
# naming the dataframe columns
featureScores.columns = ['Specs','Score']  

plt.bar([i for i in range(len(fs.scores_))], fs.scores_)
plt.xlabel('Index of feature')
plt.ylabel('Feature score');
plt.title("  Figure 4. feature score for feature selection")
plt.savefig('figure4.png', bbox_inches='tight')

print("SelectKBest, 4 best features: ")
print(featureScores.nlargest(4,'Score'))  #print 4 best features




print("----------")
# PCA
print('Explained variation per principal component (PCA): {}'.format(pca.explained_variance_ratio_))

# graph for predicted test set
predicteddf = pd.DataFrame(X_test_pca, columns=["principal 1", "principal 2", "principal 3", "principal 4"])
predicteddf["Life expectancy at birth (years)"] = y_pred_pca

g = sns.pairplot(predicteddf, hue="Life expectancy at birth (years)",  vars=["principal 1", "principal 2", "principal 3", "principal 4"], markers=["o", "s", "D"])
g.fig.text(0.15, 1, 'Figure 5. Principal component analysis of life expectancy at birth', size = 12);
plt.savefig('figure5.png', bbox_inches='tight')



print("----------")
# first 4
# graph for predicted test set
first4 = ['Access to electricity', 'Adjusted savings: emission damage', 'Birth rate, crude', 'Cause of death']
print(f'First 4 features from the original dataset: {first4}')

fourdf = pd.DataFrame(X_test4, columns=first4)
fourdf["Life expectancy at birth (years)"] = y_pred4

g = sns.pairplot(fourdf, hue="Life expectancy at birth (years)",  vars=first4, markers=["o", "s", "D"])
g.fig.text(0.15, 1, 'Figure 6. Analysis of life expectancy at birth by the first 4 original features', size = 12);
plt.savefig('figure6.png', bbox_inches='tight')