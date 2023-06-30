from phe import paillier # 开源库
import random # 选择随机数
##################### 设置参数
# 明文list
message_list = [100,200,300,400,500,600,700,800,900,1000]# 消息列表
length = len(message_list) #length=10
# 生成公私钥
public_key, private_key = paillier.generate_paillier_keypair()
##################### step1，对所有明文进行加密，服务器端只存储密文
message_list_encryption=[]
for i in range(length):
    message_list_encryption.append(public_key.encrypt(message_list[i]))
##################### step2，客户端生成密文选择向量
# 客户端随机选择一个要读的位置
pos = random.randint(0,length-1)
print("pos=",pos)
print("choose value=",message_list[pos])
select_list=[]#选择向量
enc_list=[] # 选择向量对应的密文
for i in range(length):
    select_list.append(  i == pos )
    enc_list.append( public_key.encrypt(select_list[i]) )
##################### step3，密文分组
encryption_list=[]#记录了所有明文的密文分组
for i in range(length):
    temp=message_list_encryption[i].ciphertext()#当前明文的密文
    curr_encryption_list=[]#当前明文密文拆分得到的分组
    while(True):
        curr,encryption=divmod(temp,10000000000000000)# 16bits
        curr_encryption_list.append(encryption)
        temp=curr
        if(curr==0):
            break
    curr_encryption_list.reverse()
    encryption_list.append(curr_encryption_list)

#print("encrytion_list=",encryption_list[pos])
#print(" ")
#print("密文：",message_list_encryption[pos].ciphertext())
# print("type",type(encryption_list[pos][0]))
##################### step4,相乘传输
send_list=[]
for i in range(len(encryption_list[0])):
    send_list.append(0)

for i in range(len(encryption_list)):
    for j in range(len(encryption_list[i])):
        send_list[j]+=encryption_list[i][j]*enc_list[i]
#print("sendlist=",send_list)
##################### step5，客户端收到分组，进行分组解密 
#解密密文分片
decode_list=[]
for i in range(len(send_list)): 
    decode_list.append(private_key.decrypt(send_list[i]))
##################### step6，客户端收到结果，拼接解密
res = 0
for i in range(len(send_list)):
   res=res*10000000000000000+decode_list[i]
raw_plaintext=private_key.raw_decrypt(res)
print("raw plaintext=",raw_plaintext)
