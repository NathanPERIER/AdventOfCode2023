
days = ['01', '02', '03', '04', '05']


foreach day: days
    executable(
	'day-' + day, ['day-' + day + '.cc'], 
		dependencies: [utils_dep],
		cpp_args: [],
		link_args: [],
		install: false,
		build_by_default: true
	)
endforeach
