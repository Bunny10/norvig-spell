#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cstring>
#include <regex>

#include "hat-trie.h"

using namespace std;

const string alphabet="abcdefghijklmnopqrstuvwxyz";

template<class It> It deletes(const string&word, It out)
{
  for (auto n=0;n<word.length();++n){
    string s=word;
    s.erase(n,1);
    *out++=s;
  }
  return out;
}

template<class It> It replaces(const string&word, It out)
{
  for (auto n=0;n<word.length();n++){
    string s = word;
    for (auto i=0;i<alphabet.length();i++){
      s[n] = alphabet[i];
      *out++ = s;
    }
  }
  return out;
}

template<class It> It inserts(const string&word, It out)
{
  auto N = word.length();
  string s;
  
  for (auto n=0;n<=N;n++){
    s.resize(N+1);
    for (auto j=0;j<n;j++)
      s[j] = word[j];
    for (auto j=n;j<N;j++)
      s[j+1] = word[j];
    for (auto i=0;i<alphabet.length();i++){
      s[n] = alphabet[i];
      *out++ = s;
    }
  }
  return out;
}

template<class It> It transposes(const string&word, It out)
{
  for (auto n=0;n<word.length()-1;n++){
    string s = word;
    swap(s[n],s[n+1]);
    *out++ = s;
 }
  return out;
}

template<class It> It edits1(const string &word,It out)
{
  out = deletes(word,out);
  out = replaces(word,out);
  out = transposes(word,out);
  out = inserts(word,out);
  return out;
}

class model {
  hattrie_t *tp;

  template <class InCont,class Out>
  Out annotate(InCont cont,Out out){
    for (auto w: cont){
      value_t val = lookup(w);
      if (!val)
        continue;
      *out++ = make_pair(w,val);
    }
    return out;
  }
  model(model &);
  model(model &&);
public:

  model() : tp(hattrie_create()) {}
  ~model() {hattrie_free(tp);}


  void update(const string &word) {
    value_t *p = hattrie_get(tp,word.c_str(),word.length());
    *p += 1;
  }

  value_t lookup(const string &word) {
    value_t *p = hattrie_tryget(tp,word.c_str(),word.length());
    return (p==NULL)?0:*p;
 }

  void train(istream &s);

  string correct(const string &);
};

string model::correct(const string &word)
{
  if (lookup(word))
    return word;
  vector<pair<string,value_t> > candidates;
  vector<string> e1;
  edits1(word,back_inserter(e1));
  annotate(e1,back_inserter(candidates));
  if (candidates.empty()){
    vector<string> e2;
    for (auto w: e1)
      edits1(w,back_inserter(e2));
    annotate(e2,back_inserter(candidates));
  }
  if (candidates.empty())
    return word;
  auto maxp = max_element(candidates.begin(),
                          candidates.end(),
                          [](const auto &a,const auto &b) 
                            {return a.second<=b.second;});
  return maxp->first;
}
  
void model::train(istream &fs)
{
  ostringstream ss;
  ss << fs.rdbuf();
  string s=ss.str();
  regex re("[[:alpha:]]+");

  auto mp = sregex_iterator(s.begin(),s.end(),re);
  decltype(mp) mend;

  for (;mp!=mend;++mp){
    string tok = mp->str(0);
    for (auto i=0;i<tok.length();i++)
      tok[i]=tolower(tok[i]);
    update(tok);
  }
}

int main(int argc,char*argv[])
{
  if (argc!=2){
    cerr<<"Usage: norvig [training dataset]\n";
    return 2;
  }
  ifstream fs(argv[1]);
  if (!fs){
    cerr<<argv[1]<<": "<<strerror(errno)<<'\n';
    return 1;
  }
  model mod;
  mod.train(fs);
  fs.close();
  
  string word;
  while (cin){
    getline(cin,word);
    if (word.empty())
      continue;
    for (auto n=0;n<word.length();n++)
      word[n] = tolower(word[n]);
    cout<<word<<", "<<mod.correct(word)<<'\n';
  }
}