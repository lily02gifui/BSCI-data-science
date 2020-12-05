'''
Elements of Data Processing Project 2, Task 1a.
Written by Aoi Fujii in May 2020.
'''

import math
import pandas as pd
from fuzzywuzzy import fuzz
from sklearn import preprocessing
from sklearn.preprocessing import MinMaxScaler
import re
pd.options.mode.chained_assignment = None



amazon_df = pd.read_csv("amazon_small.csv")
google_df = pd.read_csv("google_small.csv")

#preprocess price
def price_process(s):
    AUD_GBP = 1.86
    for i in range(len(s)):
        if type(s[i]) != str and math.isnan(s[i]):
            continue;
            
        if type(s[i]) == str:
            # remove '.' if there is more than one, left the last '.'
            num = len(re.findall("[\.]+",s[i]))
            if num > 1:
                s[i] = s[i].replace(".", "", num-1)

            #remove char and if there are gbp, multiply by 1.86(convet to aud)
            if "gbp" in s[i].lower():
                price = re.sub("gbp", "", s[i])
                price = re.sub("[\D\s\$%\^,\*\(\)#]+", "", price)
                s[i] = float(price) * AUD_GBP 

            else:
                # remove char
                price = re.sub("[a-zA-Z]+", "", s[i])
                if bool(price):
                    s[i] = float(price)
                else:
                    s[i] = float("nan")
                    
price_process(amazon_df.price)
price_process(google_df.price)


#preprocess text
def cleaner(s):
    for i in range(len(s)):
        s[i] = s[i].lower()
        s[i] = re.sub( r'[\(\),"?!\':;\\+\-*/^&%]', '', s[i])
        s[i] = re.sub(r'and', '', s[i])
        
cleaner(amazon_df.title)
cleaner(google_df.name)


s_l = []
p_l = []
a_id = []
g_id = []
a_p = []
g_p = []
border = 66

# calculate distance
for g_i in range(len(google_df)):
    for a_i in range(len(amazon_df)):
        score_name = fuzz.token_set_ratio(google_df['name'][g_i], amazon_df['title'][a_i]);
        pricediff = abs(amazon_df['price'][a_i] - google_df['price'][g_i])
        
        if score_name > border:    
            s_l.append(score_name)
            p_l.append(pricediff)
            g_id.append(google_df['idGoogleBase'][g_i])
            a_id.append(amazon_df['idAmazon'][a_i])
            a_p.append(amazon_df['price'][a_i])
            g_p.append(google_df['price'][g_i])

# convert it to series
a_s = pd.Series(a_id)
g_s = pd.Series(g_id)
s_s = pd.Series(s_l)
p_ds = pd.Series(p_l)
a_ps = pd.Series(a_p)
g_ps = pd.Series(g_p)

# store data in df
simdf = pd.DataFrame({'amazon_id': a_s,'amazon_price': a_ps, 'google_id': g_s,
                      'google_price': g_ps,'pricediff': p_ds, 'similarity': s_s})

# normalize
names = ['pricediff', 'similarity']
nomscaler = MinMaxScaler()
nom = nomscaler.fit_transform(simdf.loc[:,names])
normalized_df = pd.DataFrame(nom, columns = names)
normalized_df['idAmazon'] = simdf['amazon_id']
normalized_df['idGoogleBase'] = simdf['google_id']
normalized_df['namediff'] = normalized_df['similarity'].sub(100).abs().div(100)
normalized_df

#calculate overall differnce
f_diff_l = []
for i in range(len(normalized_df)):
    f_diff = normalized_df.namediff[i] + 0.8*normalized_df.pricediff[i]
    f_diff_l.append(f_diff)
    
f_diff_s = pd.Series(f_diff_l)

# normalize the value
scaled_fdiff = (f_diff_s-f_diff_s.min())/(f_diff_s.max()-f_diff_s.min())
normalized_df['totaldiff'] = scaled_fdiff

# this function finds the maching pair
def matching_pair(sorted_df, store_pair, threshold):
    while  ((not sorted_df.empty) and (sorted_df.iloc[0,5]  < threshold)):

        #store similar pair
        similar = sorted_df.iloc[0,2:4]
        store_pair = store_pair.append(similar, ignore_index=True)
        
        #remove the pair from the sorted_df
        #assume there are no duplicates, double matching, one matching wiht more than one 
        sorted_df = sorted_df[sorted_df.idAmazon !=  similar[0]] 
        sorted_df = sorted_df[sorted_df.idGoogleBase !=  similar[1]]
    
    return store_pair

store_pair = pd.DataFrame(columns = ['idAmazon','idGoogleBase'])
sorted_df = normalized_df.sort_values(by=['totaldiff']).reset_index(drop=True)
store_pair = matching_pair(sorted_df, store_pair, 0.1)
store_pair.to_csv('task1a.csv', index=False)
