
#ifndef BH_MODULES_ITEM_ITEM_DISPLAY_CONDITION_TYPE_H_
#define BH_MODULES_ITEM_ITEM_DISPLAY_CONDITION_TYPE_H_

namespace bh::modules::item {

enum ConditionType {
  CT_None,
  CT_LeftParen,
  CT_RightParen,
  CT_NegationOperator,
  CT_BinaryOperator,
  CT_Operand
};

}  // namespace bh::modules::item

#endif  // BH_MODULES_ITEM_ITEM_DISPLAY_CONDITION_TYPE_H_
