#include<libsnark/common/default_types/r1cs_gg_ppzksnark_pp.hpp>
#include<libsnark/gadgetlib1/pb_variable.hpp>
#include<libsnark/gadgetlib1/gadgets/basic_gadgets.hpp>
using namespace libsnark;
using namespace std;
//定义使用的有限域
typedef libff::Fr<default_r1cs_gg_ppzksnark_pp>FieldT;
//定义创建面包板的函数
protoboard<FieldT>build_protoboard(int* secret){
    //初始化曲线参数
    default_r1cs_gg_ppzksnark_pp::init_public_params();
    //定义面包板
    protoboard<FieldT>pb;
    //定义所需要用到的变量，x^3+x+5=out;
    pb_variable<FieldT> x;
    pb_variable<FieldT> sym_1;//x*x
    pb_variable<FieldT> y;//x^3
    pb_variable<FieldT> sym_2;//x^3+x
    pb_variable<FieldT> out;//x^3+x+5
    //下面将各个变量与 protoboard 连接，相当于把各个元器件插到“面包板”上。
    //allocate() 函数的第二个 string 类型变量仅是用来方便 DEBUG 时的注释，方便 DEBUG 时查看日志。
    out.allocate(pb, "out");
    x.allocate(pb, "x");
    sym_1.allocate(pb, "sym_1");
    y.allocate(pb, "y");
    sym_2.allocate(pb, "sym_2");
    //set_input_sizes(n) 用来声明与 protoboard 连接的 public变量的个数 n。
    //在这里 n = 1，表明与 pb 连接的前 n = 1 个变量是 public 的，其余都是 private 的。
    //因此，要将public的变量先与pb连接
    pb.set_input_sizes(1);
    //为公有变量赋值
    pb.val(out)=35;
    pb.val(sym_1) = 9;
    pb.val(y) = 27;
    pb.val(sym_2) = 30;
    if(secret!=NULL)
    { // 银行在prove阶段传入secret，其他阶段为NULL
        pb.val(x)=*secret;
    }
    //接下来添加约束
    // x*x = sym_1
    pb.add_r1cs_constraint(r1cs_constraint<FieldT>(x, x, sym_1));
    // sym_1 * x = y
    pb.add_r1cs_constraint(r1cs_constraint<FieldT>(sym_1, x, y));
    // y + x = sym_2
    pb.add_r1cs_constraint(r1cs_constraint<FieldT>(y + x, 1, sym_2));
    // sym_2 + 5 = ~out
    pb.add_r1cs_constraint(r1cs_constraint<FieldT>(sym_2 + 5, 1, out));
    
    return pb;
}
