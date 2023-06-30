from phe import paillier # 开源库
import random # 选择随机数
##################### 设置参数
# 明文list
message_list = [100,200,300,400,500,600,700,800,900,1000]# 消息列表
length = len(message_list) #length=10
# 生成公私钥
public_key, private_key = paillier.generate_paillier_keypair()
# 客户端随机选择一个要读的位置
pos = random.randint(0,length-1)
print("pos=",pos)
print("choose value=",message_list[pos])
select_list=[]#选择向量
enc_list=[] # 选择向量对应的密文
for i in range(length):
    select_list.append(  i == pos )
    enc_list.append( public_key.encrypt(select_list[i]) )
print("要读起的数值位置为：",pos)
print("明文为：",message_list[pos])

# print("密文类型",type(enc_list))
##################### 服务器端进行运算
c=0
for i in range(length):
    c = c + message_list[i] * enc_list[i]
print("产生密文：",c.ciphertext())# c.ciphertext() int
#print("length of encryption:",len(str(c.ciphertext())))
# print("密文类型：",type(c.ciphertext()))
##################### 服务器端分组密文分组传输
encryption_list=[]
temp=c.ciphertext()
while(True):
    curr,encryption=divmod(temp,10000000000000000)# 16bits
    encryption_list.append(encryption)
    temp=curr
    if(curr==0):
        break
encryption_list.reverse()
print("密文分组：",encryption_list)

#############客户端进行解密，拥有私钥
res=0
temp=""
for i in range(len(encryption_list)):
   res=res*10000000000000000+encryption_list[i]
#print("=?",res==c.ciphertext()) #true

raw_plaintext=private_key.raw_decrypt(res)
print("raw plaintext=",raw_plaintext)
