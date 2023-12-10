
days = [
	                        '01', '02', '03', 
	'04', '05', '06', '07', '08', '09', '10'
]


foreach day: days
    executable(
	'day-' + day, ['day-' + day + '.cc'], 
		dependencies: [utils_dep],
		cpp_args: ['-fopenmp'],
		link_args: ['-fopenmp'],
		install: false,
		build_by_default: true
	)
endforeach
