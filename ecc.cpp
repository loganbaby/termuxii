#include "ecc.h"

#include <cmath>

#include "exceptions.h"

#if DEBUG
#include <iostream>
#endif

EllipticCurveCryptography::FieldElement::FieldElement(const int& number, const int& prime) {
  try {
    if (number >= prime) throw OverrideExceptions::NumberNotInField();

    this->number = number;
    this->prime = prime;
  } catch (OverrideExceptions::NumberNotInField& exception) {
#if DEBUG
    std::cerr << exception.what() << std::endl;
#endif
    std::exit(EXCEPTION_NUMBER_NOT_IN_FIELD_CODE);
  }
}

EllipticCurveCryptography::FieldElement::~FieldElement() {}

bool EllipticCurveCryptography::FieldElement::operator==(std::shared_ptr<FieldElement> other) {
  if (other == nullptr) return false;

  return other->number == this->number && other->prime == this->prime;
}

bool EllipticCurveCryptography::FieldElement::operator!=(std::shared_ptr<FieldElement> other) { return !(other.get() == this); }

std::shared_ptr<EllipticCurveCryptography::FieldElement> EllipticCurveCryptography::FieldElement::operator+(
    std::shared_ptr<EllipticCurveCryptography::FieldElement> other) {
  try {
    if (this->prime != other->prime) throw OverrideExceptions::AdditionInDifferentFields();
  } catch (OverrideExceptions::AdditionInDifferentFields& exception) {
#if DEBUG
    std::cerr << exception.what() << std::endl;
#endif
    std::exit(EXCEPTION_ADD_IN_DIFF_FIELDS_CODE);
  }

  return std::make_shared<FieldElement>((this->number + other->number) % this->prime, this->prime);
}

std::shared_ptr<EllipticCurveCryptography::FieldElement> EllipticCurveCryptography::FieldElement::operator-(
    std::shared_ptr<FieldElement> other) {
  try {
    if (this->prime != other->prime) throw OverrideExceptions::SubInDifferentFields();
  } catch (OverrideExceptions::SubInDifferentFields& exception) {
#if DEBUG
    std::cerr << exception.what() << std::endl;
#endif
    std::exit(EXCEPTION_SUB_IN_DIFF_FIELDS_CODE);
  }

  if ((this->number - other->number) % this->prime >= 0) {
    return std::make_shared<FieldElement>((this->number - other->number) % this->prime, this->prime);
  } else {
    return std::make_shared<FieldElement>(this->prime - std::abs(this->number - other->number), this->prime);
  }
}

std::shared_ptr<EllipticCurveCryptography::FieldElement> EllipticCurveCryptography::FieldElement::operator*(
    std::shared_ptr<FieldElement> other) {
  try {
    if (this->prime != other->prime) throw OverrideExceptions::MulInDifferentFields();
  } catch (OverrideExceptions::MulInDifferentFields& exception) {
#if DEBUG
    std::cerr << exception.what() << std::endl;
#endif
    std::exit(EXCEPTION_MUL_IN_DIFF_FIELDS_CODE);
  }

  return std::make_shared<FieldElement>((this->number * other->number) % this->prime, this->prime);
}

std::shared_ptr<EllipticCurveCryptography::FieldElement> EllipticCurveCryptography::FieldElement::operator/(
    std::shared_ptr<FieldElement> other) {
  try {
    if (this->prime != other->prime) throw OverrideExceptions::TrueDivInDifferentFields();
  } catch (OverrideExceptions::TrueDivInDifferentFields& exception) {
#if DEBUG
    std::cerr << exception.what() << std::endl;
#endif
    std::exit(EXCEPTION_MUL_IN_DIFF_FIELDS_CODE);
  }

  return std::make_shared<FieldElement>(
      this->number * static_cast<int>(trueDivModuleExp(other->number, this->prime - 2, this->prime)) % this->prime, this->prime);
}

std::shared_ptr<EllipticCurveCryptography::FieldElement> EllipticCurveCryptography::FieldElement::Pow(const int& exponent) {
  return std::make_shared<FieldElement>(static_cast<int>(std::pow(this->number, exponent)) % this->prime, this->prime);
}

void EllipticCurveCryptography::FieldElement::PrintFieldElement() {
  std::cout << "FieldElement: Number: " << this->number << " Prime: " << this->prime << std::endl;
}

int EllipticCurveCryptography::FieldElement::trueDivModuleExp(const int& x, const int& y, const int& N) {
  if (y == 0) return 1;

  int z = trueDivModuleExp(x, y / 2, N);

  if (y % 2 == 0)
    return (z * z) % N;
  else
    return (x * z * z) % N;
}
