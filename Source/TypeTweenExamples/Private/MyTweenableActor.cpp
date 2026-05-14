// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTweenableActor.h"

#include "TypeTween.h"

#include "Math/Transform.h"


// Sets default values
AMyTweenableActor::AMyTweenableActor() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyTweenableActor::BeginPlay() {
	Super::BeginPlay();

	TypeTween::Tween(this)
		.MoveBy(Offset.GetTranslation())
		.Preset(MovePreset);

	TypeTween::Tween(this)
		.RotateBy(Offset.GetRotation())
		.Preset(RotatePreset);

	TypeTween::Tween(this)
		.ScaleBy(Offset.GetScale3D())
		.Preset(ScalePreset);
}

// Called every frame
void AMyTweenableActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AMyTweenableActor::CompileChecks() {
	/* ===== Some Tweens to make sure it all compile ===== */

	// =============
	// Tween Types
	// =============
	{
		/* Pointer Tween */
		{
			/* Takes in an external pointer to a type and updates it */
			float my_float = 0.0f;
			TypeTween::Tween(my_float, this)
				.From(0.0f)
				.To(10.0f)
				.Duration(2.5f)
				.Ease(ETweenEase::Linear);
		}

		/* Internal Tween */
		{
			/* Creates an internal value and updates it, no need to store it yourself */
			/* Useful for when you just want a timer or alpha without an actual value to update */
			
			TypeTween::Tween<float>(this)
				.From(0.0f)
				.To(10.0f)
				.Duration(2.5f)
				.Ease(ETweenEase::Linear)
				.OnUpdate([&](const float& value) {
					// Every tick...
				})
				.OnComplete([]() {
					// Done after 2.5 sec
				});
		}

		/* Type Deduction Tween */
		{
			/* If you provide an initial value, the tween can deduce the type and you don't have to specify it */
			/* Also set the start value to provided value by default */
			TypeTween::Tween(3.14f, this)
				.From(0.0f) /* Start at 0 */
				.To(1.0f); /* End at 1 */

			/* Or */
			TypeTween::Tween(3.14f, this) /* Start at 3.14f */
				.By(1.0f); /* End at 4.14f */
		}

		/* No Value Tween (aka void tween) */
		{
			/* Just a timer with easing and callbacks, no value */
			TypeTween::Tween<void>(this)
				.Duration(0.5f)
				.OnUpdate([](float t) {
				UE_LOG(LogTemp, Log, TEXT("[Void Tween] t: %f"), t);
					});
		}
	}

	// =============
	// Supported Types
	// =============
	{
		TypeTween::Tween<float>(this).From(0.0f).To(1.0f);
		TypeTween::Tween<double>(this).From(0.0).To(1.0);
		TypeTween::Tween<int>(this).From(0).To(100);
		TypeTween::Tween<FVector>(this).From(FVector::ZeroVector).To(FVector(1.f, 2.f, 3.f));
		TypeTween::Tween<FRotator>(this).From(FRotator::ZeroRotator).To(FRotator(45.f, 90.f, 180.f));
	}

	// =============
	// Tween Options
	// ============
	{
		/* Default options */
		{
			/* Options that every tween has */
			TypeTween::Tween(this)
				.Duration(0.5f)
				.Ease(ETweenEase::InOutSine)
				.PingPong()
				.Repeat(3);
		}
		/* Typed options*/
		{
			/* Options that you only have for typed tweens */
			/* If you provide a float, you get float setters */
			TypeTween::Tween(0.f, this)
				.From(0.f)
				.To(1.f)
				.By(1.f); /* ``By`` is relative to the start value if no ``To`` is called */

			/* If you provide a FVector, you get FVector setters */
			TypeTween::Tween<FVector>(this)
				.From(FVector::ZeroVector)
				.To(FVector(1.f, 2.f, 3.f))
				.By(FVector(1.f, 1.f, 1.f));

			/* If you provide no value, you get no typed setters */
			TypeTween::Tween(this)
				// .From(...) /* Error: no From for void tween */
				// .To(...)   /* Error: no To for void tween */
				.Duration(0.5f);
		}
	}

	// =============
	// Delays
	// =============
	{
		/* Delays work for all tweens, give you more control */
		TypeTween::Tween(this)
			.StartDelay(0.2f)   /* Wait 0.2s before starting the tween */
			.ReverseDelay(0.1f) /* [Ping Pong Only] Wait 0.1s before reversing */
			.RepeatDelay(0.4f)  /* [Repeat count > 0]Wait 0.4s before repeating */
			.EndDelay(0.3f);    /* Wait 0.3s after finishing */
	}

	// =============
	// Callbacks
	// =============
	{
		/* Typed Callbacks */
		{
			/* in ``OnUpdate`` you get the current value, in addition to the alpha, if you have a typed tween */
			TypeTween::Tween(0.f, this)
				.OnUpdate(
					[](float t, const float& value) {
						UE_LOG(LogTemp, Log, TEXT("[Typed Callback] t: %f, value: %f"), t, value);
					}
				);

			/* If you have no value, you get simpler callbacks without the value parameter */
			TypeTween::Tween<void>(this)
				.OnUpdate(
					[](float t) {
						UE_LOG(LogTemp, Log, TEXT("[Untyped Callback] t: %f"), t);
					}
				);

			/* This works for any type */
			/* FVector */
			TypeTween::Tween(FVector::ZeroVector, this)
				.OnUpdate(
					[](float t, const FVector& value) {
						UE_LOG(LogTemp, Log, TEXT("[Typed Callback] t: %f, value: %s"), t, *value.ToString());
					}
				);

			/* FRotator */
			TypeTween::Tween(FRotator::ZeroRotator, this)
				.OnUpdate(
					[](float t, const FRotator& value) {
						UE_LOG(LogTemp, Log, TEXT("[Typed Callback] t: %f, value: %s"), t, *value.ToString());
					}
				);
		}
		/* Default callbacks */
		{
			/* Called in this order:*/
			TypeTween::Tween<void>(this)
				.OnPreStart([]() {})
				.OnStart([]() {})
				.OnCycleBegin([]() {})
				// .OnUpdate([]() {}) /* Depends on type and gets called when value interpolates (not during delays) */
				.OnForwardEnd([]() {})
				.OnReverseBegin([]() {}) /* [Ping Pong Only] */
				/* OnTick + OnUpdate */
				.OnCycleEnd([]() {})
				.OnRepeat([]() {}) /* Only if Repeat > 0, goes back to OnCycleBegin */
				.OnComplete([]() {})
				.OnTick([]() {}); /* Gets called every tick, even during delays */
		}
	}

	// =============
	// Handles
	// =============
	{
		/* Store type erased tween */
		{
			TypeTween::FTweenHandle Handle;
			Handle = TypeTween::Tween<float>(this).From(0.f).To(1.f);
			/* Control */
			Handle->Pause();
			Handle->Resume();
			Handle->Restart();
			Handle->Finish();
			/* Settings */
			Handle->GetSettings().Ease = ETweenEase::InOutQuad;
			Handle->GetSettings().Duration = 2.f;
			/* Callbacks */
			Handle->GetCallbacks().OnStart([]() {
				UE_LOG(LogTemp, Log, TEXT("[Handle Callback] Tween Started"));
				});
			// Handle->
		}
		/* Store typed tween */
		{
			TypeTween::TweenHandle<float> Handle;
			Handle = TypeTween::Tween<float>(this).From(0.f).To(1.f);
			/* Access typed functions */
			Handle->From(1.f).To(1.f);
			Handle->OnUpdate([](float t, const float& value) {
				UE_LOG(LogTemp, Log, TEXT("[Typed Handle Callback] t: %f, value: %f"), t, value);
				});
		}
		/* Store type erased in containers */
		{
			TArray<TypeTween::FTweenHandle> TweenHandles;
			TweenHandles.Add(TypeTween::Tween<float>(this).From(0.f).To(1.f));
			TweenHandles.Add(TypeTween::Tween<int>(this).From(1).To(100));
			TweenHandles.Add(TypeTween::Tween<FVector>(this).From(FVector::ZeroVector).To(FVector(1.f, 2.f, 3.f)));
			for (const auto& Handle : TweenHandles) {
				Handle->Pause();
			}
		}
		/* Store typed in containers */
		{
			TArray<TypeTween::TweenHandle<float>> FloatTweenHandles;
			FloatTweenHandles.Add(TypeTween::Tween<float>(this).From(0.f).To(1.f));
			FloatTweenHandles.Add(TypeTween::Tween<float>(this).From(1.f).To(100.f));
			/* Error: can't add int tween to float tween container: */
			//FloatTweenHandles.Add(TypeTween::Tween<int>(this).From(1).To(100)); 
			for (const auto& Handle : FloatTweenHandles) {
				Handle->From(0.f).To(1.f);
			}
		}
	}

	// =============
	// Transform Tween
	// =============
	{
		/* Start and End */
		{
			FTransform MyTransform;

			FTransform Start = FTransform::Identity;
			FTransform End = FTransform::Identity;
			End.SetRotation(FQuat(FRotator(0.f, 90.f, 0.f)));
			End.SetTranslation(FVector(100.f, 0.f, 0.f));
			End.SetScale3D(FVector(2.f, 2.f, 2.f));

			TypeTween::Tween(MyTransform, this)
				.From(Start)
				.To(End)
				.Ease(ETweenEase::InOutSine)
				.Duration(2.f)
				.OnUpdate([](float t, const FTransform& value) {
				UE_LOG(LogTemp, Log, TEXT("[Transform Tween] t: %f, value: %s"), t, *value.ToString());
					});
		}
		/* From/To by component */
		{
			TypeTween::Tween<FTransform>(this)
				.MoveFrom(FVector(0.f, 0.f, 0.f))
				.MoveTo(FVector(100.f, 0.f, 0.f))
				.RotateFrom(FRotator::ZeroRotator)
				.RotateTo(FRotator(0.f, 90.f, 0.f))
				.ScaleFrom(FVector(1.f, 1.f, 1.f))
				.ScaleTo(FVector(2.f, 2.f, 2.f))
				.Ease(ETweenEase::InOutSine)
				.Duration(2.f)
				/* Use callback to set position */
				.OnUpdate([this](float t, const FTransform& value) {
				SetActorTransform(value);
					});
		}
		/* Actor directly */
		{
			TypeTween::Tween(this)
				.MoveFrom(FVector(0.f, 0.f, 0.f))
				.MoveTo(FVector(100.f, 0.f, 0.f))
				.RotateFrom(FRotator::ZeroRotator)
				.RotateTo(FRotator(0.f, 90.f, 0.f))
				.ScaleFrom(FVector(1.f, 1.f, 1.f))
				.ScaleTo(FVector(2.f, 2.f, 2.f))
				.Ease(ETweenEase::InOutSine)
				.Duration(2.f);
			/* Tween will automatically set actor transform for you */
		}
	}

	// =============
	// Color Tween
	// =============
	{
		/* FLinearColor */
		{
			FLinearColor MyColor;
			TypeTween::Tween(MyColor, this)
				.From(FLinearColor::Yellow)
				.To(FLinearColor::Blue)
				/* Specify color interpolation mode: */
				.ColorSpace(EColorLerpMode::HSV)
				//.ColorSpace(EColorLerpMode::Oklab)
				//.ColorSpace(EColorLerpMode::sRGB)
				//.ColorSpace(EColorLerpMode::Linear)
				.Ease(ETweenEase::InOutSine)
				.Duration(2.f)
				.OnUpdate([](float t, const FLinearColor& value) {
				UE_LOG(LogTemp, Log, TEXT("[Color Tween] t: %f, value: %s"), t, *value.ToString());
					});
		}
	}

	// =============
	// FText
	// =============
	{
		/* FText with custom lerp */
		{
			FText MyText;
			TypeTween::Tween<FText>(this)
				.From(FText::FromString("Hello"))
				.To(FText::FromString("World"))
				/* Specify Lerp Mode */
				.Mode(ETextLerpMode::Reveal)
				.Mode(ETextLerpMode::Scramble)
				.Mode(ETextLerpMode::DeleteAndType)
				.Mode(ETextLerpMode::EditDistance)
				.Mode(ETextLerpMode::CharCode)
				.Ease(ETweenEase::InOutSine)
				.Duration(2.f)
				.OnUpdate([](float t, const FText& value) {
				UE_LOG(LogTemp, Log, TEXT("[FText Tween] t: %f, value: %s"), t, *value.ToString());
					});
		}
	}
}

//static_assert(TCustomLerp<FTransform>::Value, "type should have custom lerp");