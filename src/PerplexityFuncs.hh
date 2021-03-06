// Functions for calculating perplexity
#ifndef PERPLEXITY_HH
#define PERPLEXITY_HH

#include <iostream>
#include <deque>
#include <string>
#include <vector>
#include <math.h>

#include "io.hh"
#include "NGram.hh"

class Perplexity {
public:
  Perplexity(const char *);
  Perplexity(NGram *lm, const std::string ccs_name, 
	     const std::string wb_name, const std::string mb_name, 
	     const std::string unk_symbol);
  Perplexity(const std::string lm_name, const int type, 
	     const std::string ccs_name, const std::string wb_name, 
	     const std::string mb_name, const std::string unk_symbol, 
	     const int hashgram);
  ~Perplexity();

  double logprob_file(FILE *in, FILE *out, const int interval=1);
  float logprob(const char *word, float &cur_word_lp);
  inline float token_logprob(const char *word) { float tmp; return logprob(word, tmp); };
  float sentence_logprob(const char *sentence);
  void print_hitrates(FILE *out);
  double print_results(FILE *out);  
  void print_results_sami(FILE *out);
  
  enum wb_type { EVERYTIME=0, LISTED=1, MB_LISTED=2 };

  void set_wb_type(wb_type type) {m_wb_type=type;}
  void set_unk_warn(bool w) {m_print_unk_warn=w;}
  bool bryan_wc;

  // For interpolation
  inline void set_alpha(float a) {assert(a>=0 && a<=1.0);m_alpha=a;}
  // The interpolated language models must have the same vocabulary !
  void set_interpolation(std::string lm_name);
  inline void set_init_hist(int i) {m_init_hist=m_cur_init_hist=i;}

  // For interspeech2010 analysis
  void reset_hitrates();
  int get_hitorder(int i);

  void clear_history() { history.clear(); m_cur_init_hist=m_init_hist; }

private:
  NGram *m_lm;
  NGram *m_lm2;
  std::vector<int> ccs_vector;
  std::vector<int> wb_vector;
  std::vector<std::string> mb_vector;
  std::deque<int> history;
  std::vector<int> ngram_hits; 

  void find_indices(const std::string,std::vector<int> &);
  void load_mbs(const std::string, std::vector<std::string> &);
  void init_variables();
  void init_special_symbols(const std::string ccs_name, 
			    const std::string wb_name,
			    const std::string mb_name);
  char *m_tmpstring;
  int m_tmpstring_length;
  wb_type m_wb_type;
  bool is_wb(int idx);
  bool is_mb(std::string w);
  bool m_print_unk_warn;
  bool m_need_destruct_m_lm;
  int m_init_hist;
  int m_cur_init_hist;

  /* Statistics */
  double m_logprob;
  double m_word_logprob;
  bool m_use_unk;
  bool m_use_ccs; /* use_ccs=1 not implemented... */
  int m_num_unks; 
  int m_num_tunks;
  int m_num_ccs;
  int m_num_pwords;
  int m_num_ptokens;
  int m_num_sent_ends;
  double m_perplexity;
  double m_perplexity_wo_sent_ends;
  double m_token_perplexity;

  // For interpolations
  float m_alpha;

  // For getting unknown words correct with subword models
  bool m_unkflag;
  float m_cw_lpsum;
};

#endif
