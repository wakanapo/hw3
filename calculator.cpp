#include <cstdio>
#include <iostream>

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

Formula* tokenize(std::string* str);

double read_number(std::string str, std::string::size_type pos_start,
                   std::string::size_type pos_end) {
  std::string number_str = (pos_end == std::string::basic_string::npos) ?
    str.substr(pos_start):
    str.substr(pos_start, pos_end - pos_start);
  return std::stod(number_str, nullptr);
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

Formula* make_brackets_node(Formula* formula, std::string* str) {
  if ((*str).front() == '+') {
    *str = (*str).substr(2);
    return make_plus_node(formula, tokenize(str));
  }
  if ((*str).front() == '-') {
    *str = (*str).substr(2);
    return make_minus_node(formula, tokenize(str));
  }
  if ((*str).front() == '*') {
    *str = (*str).substr(2);
    return make_multiply_node(formula, tokenize(str));
  }
  if ((*str).front() == '/') {
    *str = (*str).substr(2);
    return make_divide_node(formula, tokenize(str));
  }
  *str = (*str).substr(1);
  return tokenize(str);
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

Formula* tokenize(std::string* str) {
  Formula* formula = new Formula(NUMBER, 0);
  std::string::size_type pos_operator_before = 0;
  std::string::size_type pos_operator_after = 0;
  double num;
  *str = '+' + *str;
  
  while (pos_operator_before + 1 <= (*str).length()) {
    if ((*str).front() == ')') {
      *str = (*str).substr(1);
      break;
    }
    pos_operator_after = (*str).find_first_of("+-*/()", pos_operator_before + 1);
    if (pos_operator_after != std::string::basic_string::npos &&
        (*str).at(pos_operator_after) == '(') {
      *str = (*str).substr(pos_operator_before);
      formula = make_brackets_node(formula, str);
      pos_operator_before = 0;
      continue;
    }
    
    if (pos_operator_after - pos_operator_before == 1) {
      pos_operator_after = (*str).find_first_of("+-*/)", pos_operator_after + 1);
    }
    num = read_number(*str, pos_operator_before + 1, pos_operator_after);
    if ((*str).at(pos_operator_before) == '+' ||
        ((*str).at(pos_operator_before) == '('))
      formula = make_plus_node(formula, new Formula(NUMBER, num));
    if ((*str).at(pos_operator_before) == '-')
      formula = make_minus_node(formula, new Formula(NUMBER, num));
    if ((*str).at(pos_operator_before) == '*')
      formula = make_multiply_node(formula, new Formula(NUMBER, num));
    if ((*str).at(pos_operator_before) == '/')
      formula = make_divide_node(formula, new Formula(NUMBER, num));
    if (pos_operator_after == std::string::basic_string::npos)
      break;
    if ((*str).at(pos_operator_after) == ')') {
      *str = (*str).substr(pos_operator_after + 1);
      break;
    }
    pos_operator_before = pos_operator_after;
   
  }
  return formula;
}

int main() {
  std::string str;
  std::cin >> str;
  Formula* formula = tokenize(&str);
  std::cout << evaluate(formula) << std::endl;
  return 0;
}
