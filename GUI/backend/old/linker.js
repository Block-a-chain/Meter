function get_data(){
	var python = require("python-shell")
	var path = require("path")

	var key = document.getElementById("device_id");

	var options = {
		scriptPath : path.join(_dirname,''),
		args : [key]
	}
	var data = new python('user.py',options);

	data.on('message',function(message) {
		swal(message);
	})
	document.getElementById("login").classList.add("login_style_add");
}
