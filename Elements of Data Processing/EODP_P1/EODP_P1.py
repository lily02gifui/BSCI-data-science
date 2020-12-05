'''
Elements of Data Processing, Assignment1, Aoi Fujii

Reference:
2020s1_workshop4_solutions.pynb (https://canvas.lms.unimelb.edu.au/courses/12012/files/2938233/download?download_frd=1)
2020S1-workshop-week2-lab-solutions-1.ipynb (https://canvas.lms.unimelb.edu.au/courses/12012/files/2724784/download?download_frd=1)
'''

'''
Task 1: Crawl http://comp20008-jh.eng.unimelb.edu.au:9889/main/
to find a complete list of articles available.
Produce 'task1.csv' containing the URL and headline of each article.
'''
import requests
from bs4 import BeautifulSoup
from urllib.parse import urljoin
import pandas as pd
import re

# Specify the initial page to crawl
base_url = 'http://comp20008-jh.eng.unimelb.edu.au:9889/main/'
seed_item = 'index.html'

seed_url = base_url + seed_item
page = requests.get(seed_url)
soup = BeautifulSoup(page.text, 'html.parser')

# record visited urls
visited = {};
visited[seed_url] = True

# find the links
links = soup.findAll('a')
seed_link = soup.findAll('a', href=re.compile("^index.html"))

# to_visit_relative = list(set(links) - set(seed_link))
to_visit_relative = [l for l in links if l not in seed_link]

# get linked urls, concatenate with seed_url
to_visit = []
for link in to_visit_relative:
    to_visit.append(urljoin(seed_url, link['href']))

dfT1 = pd.DataFrame([], columns=['url', 'headline'])

# Find all outbound links on succsesor pages and explore each one
while (to_visit):

    # consume the list of urls to visit and get the page
    link = to_visit.pop(0)
    page = requests.get(link)
    soup = BeautifulSoup(page.text, 'html.parser')

    # get headline
    headline = soup.find('h1')
    pattern = r'<.*?>'
    new_headline = re.sub(pattern, '', str(headline))

    # create and add a row with url and headline
    row_df = pd.DataFrame([[link, new_headline]], columns=['url', 'headline'])
    dfT1 = pd.concat([dfT1, row_df], ignore_index=True)

    # mark the item as visited
    visited[link] = True

    # get new links in the link
    new_links = soup.findAll('a')

    # add new absolute url to to_visit list
    for new_link in new_links:
        new_item = new_link['href']
        new_url = urljoin(link, new_item)
        if new_url not in visited and new_url not in to_visit:
            to_visit.append(new_url)

# create a csv file
dfT1.to_csv('task1.csv', index=False, encoding='utf-8')

'''
Task 2a: extract the name (in tennis.json) of the first player mentioned
in the article. We will assume the article is written about that player.
'''
import nltk
from nltk.corpus import stopwords
import requests
from bs4 import BeautifulSoup
import re
import json
import pandas as pd

# get a list of urls from task1.csv
dataT1 = pd.read_csv('task1.csv', encoding='ISO-8859-1')
urlList = dataT1['url']

# if running the first time with errors:
# nltk.download('punkt')
# nltk.download('stopwords')

# get the name of players included in tennis.json
players = set()
with open("tennis.json", "r") as f:
    dataJ = json.load(f)
for i in dataJ:
    players.add(i['name'])

# store name for each url
name_dict = {}

# check each article
for url in urlList:

    # get the content of the article from the url
    page = requests.get(url)
    soup = BeautifulSoup(page.text, 'html.parser')
    body = soup.findAll('body')

    # remove links as it is related to different article
    text = re.sub(r'<div id="links">.*?</div>', '', str(body))

    # remove <> to get the text only
    pattern = r'<.*?>'
    article = re.sub(pattern, '', str(text))

    # tokenize the article
    wordList = nltk.word_tokenize(article)

    # run the line to download it the first time:
    # nltk.download('stopwords')

    # remove unnecessary words from the wordlist
    # i.e. included in stopWords, starts with lowercase and includes non-alphabetical letter
    stopWords = set(stopwords.words('english'))
    filteredList = [w.upper() for w in wordList if ((not w.lower() in stopWords)
                                                    and (w.isalpha()) and (w[0].isupper()))]

    # find the name (at most 4 words which is the length of the longest name in tennis.json) from the filtered list
    for i in range(len(filteredList)):
        for j in range(4):
            name = ' '.join(filteredList[i:i + j + 1])
            if name in players:
                name_dict[url] = name
                break;
        else:
            continue;
        break;

'''
Task 2b: extract the first complete match score identified in the article. 
We will assume this score relates to the first named player in the article.
'''

# store the score for each url
scoreDict = {}
# store the url, headline, player name, and score for each article
dfT2 = pd.DataFrame([], columns=['url', 'headline', 'player', 'score'])

# match with score with 2-5 sets
possibleScore = r'(.*?)((\d+[-/]\d+\s){2,5}).*'
# match withscore in the tie breaker
tieBreak = r'\(\d+[-/]\d+\)\s'

# find score for each url with valid player name
for url in name_dict.keys():
    # get the content of the article from the url
    page = requests.get(url)
    soup = BeautifulSoup(page.text, 'html.parser')

    # headline
    head = soup.find('h1')
    pattern = r'<.*?>'
    headline = re.sub(pattern, '', str(head))

    # get body
    body = soup.findAll('body')

    # remove links as it is related to different article
    text = re.sub(r'<div id="links">.*?</div>', '', str(body))

    # remove <>
    pattern = r'<.*?>'
    article = re.sub(pattern, '', str(text))

    # remove the score in the tie breaker
    newString = re.sub(tieBreak, '', article)
    # winnter should have 2 or 3 winning sets
    WINNING_SET = [2, 3]
    # location of the score in the extracted pattern
    SCORELOC = 2
    # check there are 2-5 sets using regex
    while re.search(possibleScore, newString):
        m = re.search(possibleScore, newString)

        # check validity of eack score
        scores = m.group(SCORELOC)
        scoreList = scores.split()

        # store the set count
        one = 0
        opponent = 0
        for game in scoreList:
            gameCount = game.split('-')
            oneScore = int(gameCount[0])
            oppScore = int(gameCount[1])

            # track set count
            if oneScore > oppScore:
                big = oneScore
                one += 1
            else:
                big = oppScore
                opponent += 1

            gameDiff = abs(oneScore - oppScore)
            if (gameDiff == 1 and big == 7) or (gameDiff == 2 and big >= 6) or (
                    gameDiff > 2 and gameDiff <= 6 and big == 6):
                continue;

            else:
                # invalid, remove the scores and find a newone
                newString = newString.replace(scores, '', 1)
                break;
        else:
            # overall score
            if ((one in WINNING_SET and opponent < one) or (opponent in WINNING_SET and one < opponent)):
                scoreDict[url] = scores[:-1]
                row_df = pd.DataFrame([[url, headline, name_dict[url], scores[:-1]]],
                                      columns=['url', 'headline', 'player', 'score'])
                dfT2 = pd.concat([dfT2, row_df], ignore_index=True)
            break;

        # if not valid, consider another numbers
        continue;

'''Task 2: Produce task2.csv containing the URL, headline, first player mentioned 
and first complete match score of each the articles.'''

dfT2.to_csv('task2.csv', index=False, encoding='utf-8')

'''
Task 3: For each article used in Task 2, identify the absolute value of the game difference.
Produce a task3.csv file containing the player name and average game difference for each
player that at least one article has been written about.
'''

import csv
import re
import pandas as pd

# get score and player name from task2.csv
dataT2 = pd.read_csv('task2.csv', encoding='ISO-8859-1')
scoreList = dataT2['score']
playerList = dataT2['player']

# store game difference using this list
gameDiffList = []
# matcing pattern to the score
oneScore = r'(\d)+[-/](\d+)'
# check the score for each article
for oneSet in scoreList:
    diff = 0
    # find game difference for each score in the set
    while re.search(oneScore, oneSet):
        m = re.search(oneScore, oneSet)
        # score of the player
        score1 = int(m.group(1))
        # score of the opponent
        score2 = int(m.group(2))

        diff += (score1 - score2)
        oneSet = oneSet.replace(m.group(), '', 1)

    # overall absolute game dfference
    gameDiff = abs(diff)
    gameDiffList.append(gameDiff)

# create a data frame of player name and game difference for each article
s_gameDiff = pd.Series(gameDiffList)
s_player = pd.Series(playerList)
gameDiff = pd.DataFrame({'player': s_player, 'avg_game_difference': s_gameDiff})

# get the aveage game difference by grouping same player and finding the mean
grouped = gameDiff.groupby('player')
ave_gameDiff = grouped.mean().reset_index()

# create csv file
ave_gameDiff.to_csv('task3.csv', index=False, encoding='utf-8')

'''
Task 4: Generate a suitable plot (task4.png) showing five players that articles are most frequently written
about and the number of times an article is written about that player.
'''

%matplotlib inline
import matplotlib.pyplot as plt
import calendar
from numpy import arange
import pandas as pd

dataT2 = pd.read_csv('task2.csv', encoding='ISO-8859-1')

# get top5 player name that articles are most frequently written about and the number of articles
top5Num = dataT2.player.value_counts().head(5)
playerName = top5Num.index

# create a bar graph
plt.bar(arange(len(playerName)), top5Num)
plt.xticks(arange(len(playerName)), playerName, rotation=25)
plt.ylabel("number of article")
plt.xlabel("player name")
plt.title("Five players that articles are most frequently written about")

# save the bargraph
plt.savefig('task4.png', bbox_inches='tight')

'''
Task 5: Generate a suitable plot (task5.png) showing the average game difference  
for each player that at least one article has been written about and their win percentage.
'''

%matplotlib inline
import pandas as pd
import matplotlib.pyplot as plt
import pandas as pd
import json

# get the name of the player and the average game difference from task3.csv
dataT3 = pd.read_csv('task3.csv', encoding='ISO-8859-1')
names = dataT3['player']
avg_game_diff = dataT3['avg_game_difference']

# store win percentage
winp = []
with open("tennis.json", "r") as f:
    dataJ = json.load(f)

# find the win percentage for each player
for name in names:
    # get the data about the player from tennis.json
    oneDataJ = list(filter(lambda x: x['name'] == name, dataJ))
    # get the percentage (removing '%' mark)
    winp.append(float(oneDataJ[0]['wonPct'][:-1]))

'''Take the value, minimum and maximun range. Check the outlier fence for the value set. Return a dictionary 
with larger outer fence and smaller outer fence within the range (Min ang Max) if there is any.'''


def outlier(values, Min=-float("inf"), Max=float("inf")):
    num_values = []
    # convert the values to float
    for i in values:
        num_values.append(float(i))
    s_num = pd.Series(num_values)

    # find the 1st, 2nd, 3rd quantile, and IQR of the data
    Q = s_num.quantile([0.25, 0.5, 0.75])
    IQR = Q[0.75] - Q[0.25]

    # find larger and smaller oute fence with in a given range
    outerf = {}
    Big = Q[0.75] + (3 * IQR)
    Small = Q[0.25] - (3 * IQR)
    if Big <= Max:
        outerf['Big'] = Q[0.75] + (3 * IQR)
    if Small >= Min:
        outerf['Small'] = Small

    return outerf;


# find the outer fence for average game differnce and win percent
out_gamediff = outlier(avg_game_diff, 0)
out_winp = outlier(winp, 0, 100)

'''Take the value1, corresponding value2 and a dictionary of outlier fence for value1 to check whether
the number in the value1 is the outlier. Value1 with no outliers, value1 with outliers and corresponding value2 for each are returned.'''


def outChecker(value1, outlier, value2):
    # store non-outliers in value1 and corresponding value2
    noOut1 = list(value1)
    noOut2 = list(value2)
    # store outliers in value1 and corresponding value2
    out1 = []
    out2 = []
    # check the outliers for each vaues in value1
    for i in range(len(value1)):
        if ('Small' in outlier.keys() and value1[i] < outlier['Small']) or (
                'Big' in outlier and value1[i] > outlier['Big']):
            noOut1.remove(value1[i])
            out1.append(value1[i])
            noOut2.remove(value2[i])
            out2.append(value2[i])

    return noOut1, out1, noOut2, out2;


# as you can see from the next 2 lines, no outliers are included in winp but in avg_gamediff
# so we mark differently for the outlier for average game difference
# print (outChecker(avg_game_diff, out_gamediff, winp))
# print(outChecker(winp, out_winp,avg_game_diff))

# store non-outliers and outliers to the corresponding variable
avg_game_diff_valid, avg_game_diff_out, winp_valid, winp_out = outChecker(avg_game_diff, out_gamediff, winp)

# create a scatter plot
# caption about outlier: refer to text and annotation at 'https://jakevdp.github.io/mpl_tutorial/tutorial_pages/tut4.html'
fig, ax = plt.subplots()
fig.text(0.5, -0.05, '*outlier is indicated by a blue point')

# plot non-outliers with red
plt.scatter(avg_game_diff_valid, winp_valid, color='red', linewidth=2, s=5)
# plot outliers with blue
plt.scatter(avg_game_diff_out, winp_out, color='blue', linewidth=2, s=5)

plt.xlim(0, 14)
plt.ylim(35, 90)
plt.xlabel("average game difference")
plt.ylabel("win percentage (%)")
plt.title("Average game difference and winnig percentage for the players\n")
plt.grid(True)

# save the scatterplot
plt.savefig('task5.png', bbox_inches='tight')

