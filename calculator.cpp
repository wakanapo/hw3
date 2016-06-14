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

std::string::size_type find_closed_parenthesis
(std::vector<std::string> token_vector, std::string::size_type pos) {
  int brackets_depth = 1;
  while (pos < token_vector.size()) {
     if (token_vector[pos] == "(")
       brackets_depth++;
     if (token_vector[pos] == ")")
       brackets_depth--;
     if (brackets_depth == 0)
       break;
     pos++;
   }
  return pos;
}

Formula* buildSyntaxTree(std::vector<std::string> token_vector) {
  Formula* formula = new Formula(NUMBER, 0);
  Formula* new_formula;
  std::string::size_type index = 1;
  std::string::size_type end = 0;
  while (index < token_vector.size()) {
    if (token_vector[index] == "(") {
      end = find_closed_parenthesis(token_vector, index + 1);
      std::vector<std::string> sub_token(token_vector.begin() + index + 1,
                                         token_vector.begin() + end);
      new_formula = buildSyntaxTree(sub_token);
    }
    if (std::any_of(token_vector[index].begin(), token_vector[index].end(), isdigit))
      new_formula = new Formula(NUMBER, read_number(token_vector[index]));
    if (token_vector[index-1] == "+")
      formula = make_plus_node(formula, new_formula);
    if (token_vector[index-1] == "-")
      formula = make_minus_node(formula, new_formula);
    if (token_vector[index-1] == "*")
      formula = make_multiply_node(formula, new_formula);
    if (token_vector[index-1] == "/")
      formula = make_divide_node(formula, new_formula);
    if (token_vector[index] == "(")
      index = end;
    index++;
  }
  return formula;
}

int main() {
  std::string str;
  std::cin >> str;
  std::vector<std::string> v = tokenize(str);
  Formula* formula = buildSyntaxTree(v);
  std::cout << evaluate(formula) << std::endl;
  return 0;
}
