'''
Elements of Data Processing Project 2, Task 1b.
Written by Aoi Fujii in May 2020.
'''

import math
import pandas as pd
import re
import numpy as np
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.metrics.pairwise import cosine_similarity
#https://scikit-learn.org/stable/modules/generated/sklearn.feature_extraction.text.CountVectorizer.html
#https://scikit-learn.org/stable/modules/generated/sklearn.metrics.pairwise.cosine_similarity.html



# get the data
amazon_df = pd.read_csv("amazon.csv")
google_df = pd.read_csv("google.csv")

# get the name of each items
a_name = amazon_df['title'].tolist()
g_name = google_df['name'].tolist()

# preprocess text
def cleaner(string):
    string = string.lower()
    string = re.sub( r'[\(\),"?!\':;\\+\-*/^&%]', '', string)
    string = re.sub(r'and', '', string)
    return string;    

# store the cleansed text
a_namelist = []
for text in a_name:
    a_namelist.append(cleaner(text))

g_namelist = []    
for text in g_name:
    g_namelist.append(cleaner(text))
    
# vectorization
ngram_vectorizer = CountVectorizer(analyzer='char_wb', ngram_range=(5, 5))
vectorize = ngram_vectorizer.fit(a_namelist)

avec = vectorize.transform(a_namelist).toarray()
gvec = vectorize.transform(g_namelist).toarray()

# calculate similarity
similarity = cosine_similarity(avec, gvec, dense_output=True)

# get the index which has higher score than the threshold
threshold = 0.45
i = np.where( similarity > threshold )
a_index = i[0]
g_index = i[1]

# store similar records
keyg = []
valueg = []
keya = []
valuea = []

for i in range(len(a_index)):
    keyg.append(amazon_df['title'][a_index[i]])
    valueg.append(google_df['id'][g_index[i]])
    keya.append(amazon_df['title'][a_index[i]])
    valuea.append(amazon_df['idAmazon'][a_index[i]])
    
gkey = pd.Series(keyg)
gval = pd.Series(valueg)
akey = pd.Series(keya)
aval = pd.Series(valuea)

# assign them to blocks
gblock = pd.DataFrame({'block_key': gkey, 'product_id': gval})
ablock = pd.DataFrame({'block_key': akey, 'product_id': aval})

ablock = ablock.drop_duplicates(subset=None, keep="first", inplace=False, ignore_index=True)
gblock = gblock.drop_duplicates(subset=None, keep="first", inplace=False, ignore_index=True)

# create csv
ablock.to_csv('amazon_blocks.csv', index=False)
gblock.to_csv('google_blocks.csv', index=False)

