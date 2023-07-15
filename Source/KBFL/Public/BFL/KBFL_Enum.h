#pragma once

UENUM(BlueprintType)
enum class KBFLFoundSomething : uint8 {
	Found UMETA(DisplayName = "Found"),
	NotFound UMETA(DisplayName = "Not Found")};

UENUM(BlueprintType)
enum class KBFLInOut : uint8 {
	Input UMETA(DisplayName = "Input"),
	Output UMETA(DisplayName = "Output"),
	Both UMETA(DisplayName = "Both")};
