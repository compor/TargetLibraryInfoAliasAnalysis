
; RUN: opt < %s -load-pass-plugin=%bindir/%testeelib -aa-pipeline=basic-aa,tli-aa -passes=aa-eval -print-all-alias-modref-info -disable-output 2>&1 | FileCheck %s

@val = external dso_local global double, align 8

; CHECK-LABEL: bar
; CHECK-DAG: NoModRef:{{ *}}tail call void @foo({{.*}}){{.*}}<->{{ *}}%call {{.*}}
; CHECK-DAG: NoModRef:  Ptr: double* @val  <->  %call = tail call double @sqrt(double %0) #3

; Function Attrs: nounwind uwtable
define dso_local i32 @bar() local_unnamed_addr #0 {
entry:
  tail call void @foo(double* nonnull @val) #3
  %0 = load double, double* @val, align 8
  %call = tail call double @sqrt(double %0) #3
  %conv = fptosi double %call to i32
  ret i32 %conv
}

declare dso_local void @foo(double*) local_unnamed_addr #1

; Function Attrs: nounwind
declare dso_local double @sqrt(double) local_unnamed_addr #2

attributes #0 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

