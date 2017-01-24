.text

.global noop
noop:
	ret

// http://billauer.co.il/blog/2014/08/wmb-rmb-mmiomb-effects/
.global wmb
wmb:
	dsb st
	ret

.global rmb
rmb:
	dsb sy
	ret
