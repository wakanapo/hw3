#include <cctype>
#include <cstdio>
#include <iostream>
#include <vector>

enum Type {
  NUMBER,
  PLUS,
  MINUS,
  MULTIPLY,
  DIVIDE
};

struct Formula {
  Type type;
  double num;
  Formula *left;
  Formula *right;

  Formula(Type type, double num) {
    this->type = type;
    this->num = num;
  }

  Formula(Type type, Formula* left, Formula* right) {
    this->type = type;
    this->left = left;
    this->right = right;
  }
};

double read_number(std::string str) {
  return std::stod(str, nullptr);
}

Formula* make_plus_node(Formula* formula, Formula* new_formula) {
  return new Formula(PLUS, formula, new_formula);
}

Formula* make_minus_node(Formula* formula, Formula* new_formula) {
  return new Formula(MINUS, formula, new_formula);
}

Formula* make_multiply_node(Formula* formula, Formula* new_formula) {
  formula->right = new Formula(MULTIPLY, formula->right, new_formula);
  return formula;
}

Formula* make_divide_node(Formula* formula, Formula* new_formula) {
  formula->right = new Formula(DIVIDE, formula->right, new_formula);
  return formula;
}

Formula* make_brackets_node(Formula* formula, Formula* brackets_formula, std::string str) {
  if (str == "+")
    return formula = make_plus_node(formula, brackets_formula);
  if (str == "-")
    return formula = make_minus_node(formula, brackets_formula);
  if (str == "*")
    return formula = make_multiply_node(formula, brackets_formula);
  if (str == "/")
    return formula = make_divide_node(formula, brackets_formula);
  return brackets_formula;
}

double evaluate(Formula* formula) {
  double ans = 0;
  if (formula->type == PLUS)
    ans = evaluate(formula->left) + evaluate(formula->right);
  if (formula->type == MINUS)
    ans = evaluate(formula->left) - evaluate(formula->right);
  if (formula->type == MULTIPLY)
    ans = evaluate(formula->left) * evaluate(formula->right);
  if (formula->type == DIVIDE)
    ans = evaluate(formula->left) / evaluate(formula->right);
  if (formula->type == NUMBER)
    ans = formula->num;
  return ans;
}

std::vector<std::string> tokenize(std::string str) {
  std::vector<std::string> token_vector;
  std::string::size_type pos_operator_before = 0;
  std::string::size_type pos_operator_after = 0;
  str = "+" + str;
  
  while (pos_operator_before + 1 <= str.length()) {
    if (str.at(pos_operator_before) == ')') {
      token_vector.push_back(str.substr(pos_operator_before, 1));
      pos_operator_before++;
      continue;
    }
      
    pos_operator_after = str.find_first_of("+-*/()", pos_operator_before + 1);
    token_vector.push_back(str.substr(pos_operator_before, 1));
    
    if (pos_operator_after == std::string::basic_string::npos) {
      token_vector.push_back(str.substr(pos_operator_before + 1));
      break;
    }
    if (str.at(pos_operator_after) == '(') {
      token_vector.push_back(str.substr(pos_operator_after, 1));
      str.replace(pos_operator_after, 1, "+");
      pos_operator_before = pos_operator_after;
      continue;
    }
    if (pos_operator_after - pos_operator_before == 1) 
      pos_operator_after = str.find_first_of("+-*/)", pos_operator_after + 1);

    token_vector.push_back(str.substr(pos_operator_before + 1,
                                      pos_operator_after - pos_operator_before - 1));
    pos_operator_before = pos_operator_after;
  }
  return token_vector;
}

Formula* buildSyntaxTree(std::vector<std::string>* token_vector) {
  Formula* formula = new Formula(NUMBER, 0);
  std::string::size_type index = 0;
  while (index < (*token_vector).size()) {
    if ((*token_vector)[index] == "(") {
      std::string ope= (*token_vector)[index-1];
      (*token_vector).erase((*token_vector).begin(), (*token_vector).begin() + index + 1);
      formula = make_brackets_node(formula, buildSyntaxTree(token_vector), ope);
      continue;
    }
    if ((*token_vector)[index] == ")") {
      (*token_vector).erase((*token_vector).begin(), (*token_vector).begin() + index + 1);
      index = 0;
      break;
    }
    if (std::any_of((*token_vector)[index].begin(), (*token_vector)[index].end(), isdigit)) {
      if ((*token_vector)[index-1] == "+")
        formula = make_plus_node(formula, new Formula(NUMBER,
                                                       read_number((*token_vector)[index])));
      if ((*token_vector)[index-1] == "-")
        formula = make_minus_node(formula, new Formula(NUMBER,
                                                        read_number((*token_vector)[index])));
      if ((*token_vector)[index-1] == "*")
        formula = make_multiply_node(formula, new Formula(NUMBER,
                                                           read_number((*token_vector)[index])));
      if ((*token_vector)[index-1] == "/")
        formula = make_divide_node(formula, new Formula(NUMBER,
                                                         read_number((*token_vector)[index])));
    }
    index++;
  }
  return formula;
}

int main() {
  std::string str;
  std::cin >> str;
  std::vector<std::string> v = tokenize(str);
  Formula* formula = buildSyntaxTree(&v);
  std::cout << evaluate(formula) << std::endl;
  return 0;
}
