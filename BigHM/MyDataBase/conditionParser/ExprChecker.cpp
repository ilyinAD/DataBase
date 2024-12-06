//
// Created by artem on 17.11.2024.
//

#include "ExprChecker.h"
#include "../utils/utils.h"

bool isBoolOp(std::string& s) {
    if (s == "&&" || s == "||" || s == "^^") {
        return true;
    }

    return false;
}

bool isCompareOp(std::string& s) {
    if (s == "=" || s == "<" || s == "!=" || s == ">" || s == ">=" || s == "<=") {
        return true;
    }

    return false;
}

bool isNumber(std::string& s) {
    for (auto i : s) {
        if (!isdigit(i)) {
            return false;
        }
    }

    return true;
}

std::shared_ptr<DataBaseType> getPureVal(std::string val, std::map<std::string, std::shared_ptr<DataBaseType>> &row) {
    if (row.find(val) != row.end()) {
        return row[val];
    }

    if (isNumber(val)) {
        return std::make_shared<Int>(Int(stoi(val)));
    }

    if (val.size() > 2 && val[0] == '0' && val[1] == 'x') {
        return std::make_shared<Bytes>(Bytes(val));
    }

    if (val == "true" || val == "false") {
        return std::make_shared<Bool>(Bool(val));
    }

    return std::make_shared<String>(String(val));
}

std::shared_ptr<DataBaseType> getByString(std::string val, std::map<std::string, std::shared_ptr<DataBaseType>> &row) {
    if (val[0] == '-') {
        int ans = *static_cast<int*>(getPureVal(val.substr(1), row)->type);
        return std::make_shared<Int>(Int(-ans));
    }

    if (val[0] == '\"') {
        return getPureVal(val.substr(1, val.size() - 2), row);
    }

    if (val[0] == '!') {
        bool ans = *static_cast<bool*>(getPureVal(val.substr(1), row)->type);
        if (ans) {
            return std::make_shared<Bool>(Bool(static_cast<std::string>("false")));
        } else {
            return std::make_shared<Bool>(Bool(static_cast<std::string>("true")));
        }
    }

    if (val.size() >= 2 && val[0] == '|' && val[val.size() - 1] == '|') {
        std::shared_ptr<DataBaseType> ans = getPureVal(val.substr(1, val.size() - 2), row);
        std::string res = *static_cast<std::string*>(ans->type);
        return std::make_shared<Int>(Int(res.size()));
    }

    return getPureVal(val, row);
}

std::shared_ptr<DataBaseType> doOp(std::string op, const std::shared_ptr<DataBaseType>& leftVal, const std::shared_ptr<DataBaseType>& rightVal) {
    DataBaseType* d;
    if (op == "*")
        d = ((*leftVal) * (*rightVal));
    else if (op == "/")
        d = ((*leftVal) / (*rightVal));
    else if (op == "%")
        d = ((*leftVal) % (*rightVal));
    else
        throw std::invalid_argument("invalid operation sign");
    std::shared_ptr<DataBaseType> sharedPtr(d);
    return sharedPtr;
}

std::shared_ptr<DataBaseType> doSimpleOp(std::string op, const std::shared_ptr<DataBaseType>& leftVal, const std::shared_ptr<DataBaseType>& rightVal) {
    DataBaseType* d;
    if (op == "+")
        d = ((*leftVal) + (*rightVal));
    else
        d = ((*leftVal) - (*rightVal));
    std::shared_ptr<DataBaseType> sharedPtr(d);
    return sharedPtr;
}

std::shared_ptr<DataBaseType> getVal(std::vector<std::string>& v, int idx, std::map<std::string, std::shared_ptr<DataBaseType>>& row) {
    if (idx >= v.size()) {
        return std::make_shared<Int>(Int(0));
    }
    if (idx == v.size() - 1) {
        return getByString(v[idx], row);
    }

    if (v[idx + 1] == "+" || v[idx + 1] == "-") {
        std::shared_ptr<DataBaseType> leftVal = getByString(v[idx], row);
        std::shared_ptr<DataBaseType> rightVal = getVal(v, idx + 2, row);
        return doSimpleOp(v[idx + 1], leftVal, rightVal);
    } else if (v[idx + 1] == "*" || v[idx + 1] == "/" || v[idx + 1] == "%") {
        std::shared_ptr<DataBaseType> cur = getByString(v[idx], row);
        while (idx + 2 < v.size()) {
            if (v[idx + 1] == "+" || v[idx + 1] == "-") {
                std::shared_ptr<DataBaseType> rightVal = getVal(v, idx + 2, row);
                return doSimpleOp(v[idx + 1], cur, rightVal);
            } else {
                cur = doOp(v[idx + 1], cur, getByString(v[idx + 2], row));
                idx += 2;
            }
        }
        return cur;
    } else {
        throw std::invalid_argument("invalid operation");
    }
}

std::shared_ptr<DataBaseType> doCompare(std::string& comp, const std::shared_ptr<DataBaseType>& leftVal, const std::shared_ptr<DataBaseType>& rightVal) {
    DataBaseType* d;
    if (comp == "<")
        d = ((*leftVal) < (*rightVal));
    else if (comp == ">")
        d = ((*leftVal) > (*rightVal));
    else if (comp == "=")
        d = ((*leftVal) == (*rightVal));
    else if (comp == "!=")
        d = ((*leftVal) != (*rightVal));
    else if (comp == "<=")
        d = ((*leftVal) <= (*rightVal));
    else if (comp == ">=")
        d = ((*leftVal) >= (*rightVal));
    else
        throw std::invalid_argument("invalid compare sign");
    std::shared_ptr<DataBaseType> sharedPtr(d);
    return sharedPtr;
}

std::shared_ptr<DataBaseType> doBoolOp(const std::string& comp, const std::shared_ptr<DataBaseType>& leftVal, const std::shared_ptr<DataBaseType>& rightVal) {
    DataBaseType* d;
    if (comp == "&&")
        d = ((*leftVal) && (*rightVal));
    else if (comp == "||")
        d = ((*leftVal) || (*rightVal));
    else if (comp == "^^")
        d = ((*leftVal) ^ (*rightVal));
    std::shared_ptr<DataBaseType> sharedPtr(d);
    return sharedPtr;
}

//string simpleExpr(const string& expr, map<string, shared_ptr<DataBaseType>>& row) {
//    string ans = "";
//    size_t idx = min(expr.find("&&"), expr.find("||"));
//    ans
//    while ()
//}

std::shared_ptr<DataBaseType> doSDNF(const std::string& s, std::map<std::string, std::shared_ptr<DataBaseType>>& row) {
    std::vector<std::string> v = splitString(s, ' ');

    v.push_back("&&");
    v.push_back("1");
    std::vector<std::string> query;
    std::string prev = "";
    std::shared_ptr<DataBaseType> res = nullptr;

    for (int i = 0; i < v.size(); ++i) {
        if (!isBoolOp(v[i])) {
            query.push_back(v[i]);
        } else {
            std::vector<std::string> query1;
            std::vector<std::string> query2;
            int flag = 0;
            std::string comp = "";
            for (int j = 0; j < query.size(); ++j) {
                if (isCompareOp(query[j])) {
                    flag = 1;
                    comp = query[j];
                    continue;
                }
                if (!flag) {
                    query1.push_back(query[j]);
                } else {
                    query2.push_back(query[j]);
                }
            }

            std::shared_ptr<DataBaseType> q1 = getVal(query1, 0, row);
            std::shared_ptr<DataBaseType> q2 = getVal(query2, 0, row);
            std::shared_ptr<DataBaseType> res1;

            if (comp != "") {
                res1 = doCompare(comp, q1, q2);
            } else {
                res1 = q1;
            }

            if (res != nullptr) {
                res = doBoolOp(prev, res, res1);
            } else {
                res = res1;
            }

            prev = v[i];
            query = {};
        }
    }
    return res;
}

std::shared_ptr<DataBaseType> checkExpr(const std::string& expr, std::map<std::string, std::shared_ptr<DataBaseType>>& row) {
    size_t cur_idx = 0;
    size_t idx = expr.find("||");
    std::vector<std::string> v = splitString(expr, ' ');
    std::shared_ptr<DataBaseType> ans = nullptr;
    int i = 0;
    std::string s;
    while (i < v.size()) {
        if (v[i] != "||") {
            s += v[i] + " ";
        } else {
            std::shared_ptr<DataBaseType> res = doSDNF(s, row);
            if (ans == nullptr) {
                ans = res;
            } else {
                ans = doBoolOp( "||", ans, res);
            }
            s = "";
        }
        ++i;
//        std::string s = expr.substr(cur_idx, idx - cur_idx - 1);
//
//        std::shared_ptr<DataBaseType> res = doSDNF(s, row);
//
//        if (ans == nullptr) {
//            ans = res;
//        } else {
//            ans = doBoolOp( "||", ans, res);
//        }
//        cur_idx += idx + 3;
//        idx = expr.find("||", cur_idx);
    }
    if (s != "") {
        std::shared_ptr<DataBaseType> res = doSDNF(s, row);
        if (ans == nullptr) {
            ans = res;
        } else {
            ans = doBoolOp( "||", ans, res);
        }
    }

    return ans;
}



std::shared_ptr<DataBaseType> parseExpr(const std::string& s, std::map<std::string, std::shared_ptr<DataBaseType>>& row) {
    deleteDoubleSpaces(s);
    std::string ans = "";
    int i = 0;
    int quoteCnt = 0;
    while (i < s.size()) {
        if (s[i] == '\"') {
            quoteCnt++;
            quoteCnt %= 2;
        }
        if (s[i] != '(' || quoteCnt == 1) {
            ans += s[i];
            ++i;
            continue;
        }
        int cnt = 1;
        std::string subExpr = "";
        ++i;
        int l = i;
        while (i < s.size() && cnt != 0) {
            if (s[i] == '(') {
                cnt++;
            } else if (s[i] == ')') {
                cnt--;
            }
            ++i;
        }
        if (cnt != 0) {
            throw std::runtime_error("invalid expr");
        }
        subExpr = s.substr(l, i - l - 1);
        std::shared_ptr<DataBaseType> bracket_val = parseExpr(subExpr, row);
        ans += getStringByType(bracket_val);
    }
    return checkExpr(ans, row);
}

std::shared_ptr<DataBaseType> ExprChecker::getValFromExpr(const std::string &expr, std::vector<std::shared_ptr<DataBaseType>> row,
                                                          std::vector<Col> columns) {
    std::map<std::string, std::shared_ptr<DataBaseType>> mapRow;
    for (int i = 0; i < columns.size(); ++i) {
        mapRow[columns[i].name] = row[i];
    }
    try {
        //shared_ptr<DataBaseType> res = checkExpr(expr, mapRow);
        std::shared_ptr<DataBaseType> res = parseExpr(expr, mapRow);
        return res;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        throw;
    }
}

bool ExprChecker::check(const std::string &expr, std::vector<std::shared_ptr<DataBaseType>> row, std::vector<Col> columns) {
    std::map<std::string, std::shared_ptr<DataBaseType>> mapRow;
    for (int i = 0; i < columns.size(); ++i) {
        mapRow[columns[i].name] = row[i];
    }
    try {
        std::shared_ptr<DataBaseType> res = parseExpr(expr, mapRow);
        if (dynamic_pointer_cast<Bool>(res)) {
            return *(static_cast<bool*>(res.get()->type));
        }
       throw std::invalid_argument("result is not bool and not int");
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        throw;
    }
}
