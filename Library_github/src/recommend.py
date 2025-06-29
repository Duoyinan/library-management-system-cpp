//用之前捏忘了设置api key在命令行  export DEEPSEEK_API_KEY=sk-xxxxxxx
import sys
import json
import requests
import os
from collections import Counter

BOOKS_FILE = "data/books.txt"
RECORDS_FILE = "data/records.txt"

def load_books():
    books = []
    with open(BOOKS_FILE, 'r', encoding='utf-8') as f:
        for line in f:
            parts = line.strip().split(',')
            books.append({'id': parts[0], 'title': parts[1], 'author': parts[2]})
    return books

def load_records():
    records = []
    with open(RECORDS_FILE, 'r', encoding='utf-8') as f:
        for line in f:
            parts = line.strip().split(',')
            records.append({'user': parts[1], 'book': parts[2]})
    return records

def recommend_hot(books, records):
    counter = Counter([r['book'] for r in records])
    hot_ids = [b for b, _ in counter.most_common(5)]
    return [b for b in books if b['id'] in hot_ids]

def recommend_by_user(username, books, records):
    his = [r['book'] for r in records if r['user'] == username]
    if not his:
        return []
    book_map = {b['id']: b for b in books}
    fav_authors = [book_map[bid]['author'] for bid in his if bid in book_map]
    fav = Counter(fav_authors).most_common(1)
    if not fav:
        return []
    fav_author = fav[0][0]
    return [b for b in books if b['author'] == fav_author and b['id'] not in his][:5]

def recommend_ai(username, books, records):
    api_key = os.getenv("DEEPSEEK_API_KEY")
    if not api_key:
        return []
    user_books = [r['book'] for r in records if r['user'] == username]
    prompt = f"""你是一名图书馆推荐助手，以下是全部图书信息：
{json.dumps(books, ensure_ascii=False)}
用户 {username} 的借阅历史是：{json.dumps(user_books)}
请基于这些信息推荐5本书，返回格式为JSON数组，每项含id、title、author。"""
    headers = {
        "Authorization": f"Bearer {api_key}",
        "Content-Type": "application/json"
    }
    payload = {
        "model": "deepseek-chat",   # 具体模型以官方文档为准
        "messages": [
            {"role": "user", "content": prompt}
        ],
        "max_tokens": 512,
        "temperature": 0.7
    }
    resp = requests.post("https://api.deepseek.com/v1/chat/completions", headers=headers, json=payload)
    if resp.status_code == 200:
        try:
            answer = resp.json()["choices"][0]["message"]["content"]
            rec_list = json.loads(answer)
            return rec_list
        except Exception:
            return []
    return []

def main():
    username = sys.argv[1]
    mode = sys.argv[2] if len(sys.argv) > 2 else "auto"
    books = load_books()
    records = load_records()
    result = []
    if mode == "hot":
        result = recommend_hot(books, records)
    elif mode == "user":
        result = recommend_by_user(username, books, records)
    elif mode == "ai":
        result = recommend_ai(username, books, records)
    elif mode == "auto":
        userrec = recommend_by_user(username, books, records)
        result = userrec if userrec else recommend_hot(books, records)
    print(json.dumps(result, ensure_ascii=False))

if __name__ == "__main__":
    main()
