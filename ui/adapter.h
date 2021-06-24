#ifndef ADAPTER_H
#define ADAPTER_H
#include<include/concreteGoods.h>
#include<QString>

/**
 * @brief 商品类型id转字符串
 * 
 */
class qtGoodsAdapter{

//        GOODS_TYPE;
//        Q_ENUM(GOODS_TYPE);
public:
        static QString toString(int gtype) {
          switch (gtype) {
              case GOODS_TYPE::Books:
                  return QString("图书");
              case GOODS_TYPE::Clothes:
                  return QString("衣服");
              case GOODS_TYPE::EGoods:
                  return QString("电子商品");
              default:
                  return QString("未知");
          }
        }
    //参考
    //https://stackoverflow.com/questions/34281682/how-to-convert-enum-to-qstring
    // QVariant::fromValue(ModelApple::Big).toString();
   private:
};

#endif // ADAPTER_H
