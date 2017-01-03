import nltk

class Analyzer():
    """Implements sentiment analysis."""
    

    def __init__(self, positives="positive-words.txt", negatives="negative-words.txt"):   #the default       
        """Initialize Analyzer."""

        fp_pos = open(positives, "r");
        self.positives = fp_pos.readlines()   
        self.positives = [word.strip('\n') for word in self.positives[35:]]  #self.positive is a list of positive words
        
        fp_neg = open(negatives, "r");
        self.negatives = fp_neg.readlines()
        self.negatives = [word.strip('\n') for word in self.negatives[35:]]
        
        fp_pos.close()
        fp_neg.close()

     
    def analyze(self, text):    #a WORD
        """Analyze text for sentiment, returning its score."""
        
        score= 0  
        tokens = nltk.word_tokenize(text)
        
        for token in tokens:
            if token.lower() in self.positives:
                score+=1

            if token.lower() in self.negatives:
                score-=1
        
        return score            #note return the score, in analyze it does the logic
        
            
        
        
           



'''
>>> import nltk
>>> sentence = """At eight o'clock on Thursday morning
... Arthur didn't feel very good."""
>>> tokens = nltk.word_tokenize(sentence)
>>> tokens
['At', 'eight', "o'clock", 'on', 'Thursday', 'morning',
'Arthur', 'did', "n't", 'feel', 'very', 'good', '.']
>>> tagged = nltk.pos_tag(tokens)
>>> tagged[0:6]
[('At', 'IN'), ('eight', 'CD'), ("o'clock", 'JJ'), ('on', 'IN'),
('Thursday', 'NNP'), ('morning', 'NN')]
'''