import hashlib
# 文档以list方式记录
files = ["Jobs went to China",
         "Apple will still be able to capture most touchscreen production this year",
         "Apple CEO Steve Jobs announced that the iPad2 will go on sale in the United States on March 11",
         "Jobs moved the world, iPhone, iPad, iPad2, one after the other",
         "Jobs ate an apple"]
words_dictionary = {} #单词词典
encry_files = [] # 加密后的文档
encry_dictionary = {} # 加密词与files对应关系

# 加密函数，调用encode
def encrypt(word):
    return hashlib.sha256(word.encode()).hexdigest()

# 陷门生成，返回加密词汇的集合
def trapdoor(keyword):
    print("生成陷门")
    encry_keywords = set() #存储加密词汇
    for word in keyword.split():
        encry_keywords.add(encrypt(word))
    return encry_keywords

# 进行检索
def search(keyword):
    print("进行检索")
    encry_keywords = trapdoor(keyword)
    matched_files = set()
    for word in encry_keywords:
        if word in encry_dictionary:
            matched_files.update(encry_dictionary[word])
    return matched_files

# 进行解密，返回检索到的文档
def decrypt(doc_id):
    print("进行解密")
    return files[doc_id]

# 构建单词词典，记录words及其对应的files
for i in range(len(files)):
    temp_words = files[i].split()
    for temp_word in temp_words:
        if temp_word not in words_dictionary:
            words_dictionary[temp_word] = set()
        words_dictionary[temp_word].add(i)

for i in range(len(files)):
    temp_words = files[i].split()
    encry_file = set() # 存储加密后的词汇
    for temp_word in temp_words:# 词汇加密放入集合
        encry_file.add(encrypt(temp_word))
    encry_files.append(encry_file)

for i in range(len(files)):
    for word in encry_files[i]:
        if word not in encry_dictionary:
            encry_dictionary[word] = set()
        encry_dictionary[word].add(i)

# 进行加密
keyword = input("搜索关键词为：")
results=search(keyword)
if results:
    for i in results:
        print("匹配文档：",decrypt(i))
else:
    print("没有匹配的文档")
