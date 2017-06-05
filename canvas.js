var file = document.getElementById('file');
var result = document.getElementById('result');
 
// File API          
if(window.File && window.FileReader && window.FileList && window.Blob) {
    function loadLocalCsv(e) {
        //          
        var fileData = e.target.files[0];
        console.log(fileData); //           
 
        // CSV             
        if(!fileData.name.match('.csv$')) {
            alert('CSV             ');
            return;
        }
 
        // FileReader                  
        var reader = new FileReader();
        //                   
        reader.onload = function() {
            var cols = reader.result.split('\n');
            var data = [];
            for (var i = 0; i < cols.length; i++) {
                data[i] = cols[i].split(',');
            }
            var insert = createTable(data);
            result.appendChild(insert);
	    draw(data);
        }
        //            
        reader.readAsText(fileData);
    }
    file.addEventListener('change', loadLocalCsv, false);
 
} else {
    file.style.display = 'none';
    result.innerHTML = 'File API                 ';
}
 
function createTable(data) {
    var table = document.createElement('table');
    for (var i = 0; i < data.length; i++) {
        var tr = document.createElement('tr');
        for (var j = 0; j < data[i].length; j++) {
            var td = document.createElement('td');
            td.innerText = data[i][j];
            tr.appendChild(td);
        }
        table.appendChild(tr);
    }
    return table;
}


function draw(data){

	var node=[];
	var canvas = document.getElementById("canvas");
	var ctx = canvas.getContext("2d");
	var i;
	var j;
	var peer;
	var txt;
	var position=[];
	var key=[];
	var key_tmp = [];
	var search_count;
	var hit_count;
	var x;
	var y;

	//console.log(node);
	//console.log(position);

	//position set
for(i=0;i<data.length-1;i++){
	
	position[i]=[Math.random()*2201+40,2000-((data[i][1])*35)+40]; //CSVの1番目からスピードのデータ
	//position[i]=[Math.random()*5001+40,6000-((data[i][1])*100)+40]; //CSVの1番目からスピードのデータ
}


//parent set
for(i=0;i<data.length-1;i++){
		node[i]=[data[i][2],data[i][3]];
}


//key array set
for(i=0;i<data.length-1;i++){
	key[i] = new Array();
}

for(i=0;i<data.length-1;i++){
	for(j=6;j<data[i].length;j++){
		key[i][j-6]=[data[i][j]];
	}
}

search_count = data[0][4];
hit_count = data[0][5];

ctx.fillStyle = '#c0c0c0';
ctx.fillRect(0,0,2600,2200);

	for(i=0;i<data.length-1;i++){
		for(j=0;j<node[i].length;j++){
			peer = node[i][j];
			if(peer != -1){

				ctx.beginPath();
				ctx.moveTo(position[i][0],position[i][1]);
				var x1 = position[i][0];
				var y1 = position[i][1];
				var x2 = position[peer][0];
				var y2 = position[peer][1];
				var x_tmp = x1 - x2;
				var x_gen = Math.abs(x_tmp);
				var y_tmp = y1 -y2;
				var y_gen = Math.abs(y_tmp);
				
				if(x1 < x2){
					x = x1 + (x_gen/2);
				}else if(x1 > x2){
					x = x2 + (x_gen/2);
				}else if(x1 == x2){
					x = x1;
				}

				if(y1 < y2){
					y = y1 + (y_gen/2);
				}else if(y1 > y2){
					y = y2 + (y_gen/2);
				}else if(y1 == y2){
					y = y1;
				}

				ctx.lineTo(x,y);
				ctx.closePath();
				ctx.strokeStyle = 'rgba(128,0,0,0.3)';
				ctx.stroke();

				ctx.beginPath();
				ctx.moveTo(x,y);
				ctx.lineTo(position[peer][0],position[peer][1]);
				//ctx.arrow(position[i][0],position[i][1],position[peer][0],position[peer][1],[0,0.8,-20,0.8,-20,3]);
				ctx.closePath();
				ctx.strokeStyle = 'rgba(0,128,0,0.3)';
				ctx.stroke();
			}
		}
	}

	for(i=0;i<data.length-1;i++){

		ctx.beginPath();
		ctx.fillStyle = 'rgba(0,255,0,0.5)';
		ctx.arc(position[i][0],position[i][1],8,0,Math.PI*2,true);
		ctx.fill();
	}

/*
	for(i=0;i<data.length-1;i++){
		for(j=0;j<node[i].length;j++){
			peer = node[i][j];
			if(peer != -1){

				ctx.beginPath();
				ctx.moveTo(position[i][0],position[i][1]);
				ctx.lineTo(position[peer][0],position[peer][1]);
				ctx.fillStyle = 'rgba(0,0,128,0.3)';
				//ctx.arrow(position[i][0],position[i][1],position[peer][0],position[peer][1],[0,0.8,-20,0.8,-20,3]);
				ctx.closePath();
				ctx.stroke();
				ctx.fill();
			}
		}
	}
*/

	for(i=0;i<data.length-1;i++){

		ctx.font = "italic 26px Arial";
		txt = data[i][0];
		ctx.fillStyle="yellow";
		ctx.fillText(txt,position[i][0]-30,position[i][1]);
		
		txt = data[i][1];
		ctx.fillStyle="red";
		ctx.fillText(txt,position[i][0]-30,position[i][1]+30);

		txt = key[i].length;
		ctx.fillStyle="blue";
		ctx.fillText(txt,position[i][0]-30,position[i][1]+60);

	}


		ctx.font = "italic 30px Arial";

		txt = 'VIEW OF FIG :';
		ctx.fillStyle="black";
		ctx.fillText(txt,2300,1935);

		txt = 'yellow : node number';
		ctx.fillStyle="yellow";
		ctx.fillText(txt,2300,1970);
		
		txt = 'red : line speed';
		ctx.fillStyle="red";
		ctx.fillText(txt,2300,2005);

		txt = 'blue : key in node';
		ctx.fillStyle="blue";
		ctx.fillText(txt,2300,2040);

		txt = 'TOTAL : ';
		ctx.fillStyle="black";
		ctx.fillText(txt,2300,2075);

		txt = 'search query : ' + search_count;
		ctx.fillStyle="red";
		ctx.fillText(txt,2300,2110);
		
		txt = 'hit : ' + hit_count;
		ctx.fillStyle="blue";
		ctx.fillText(txt,2300,2145);

		var percent = (hit_count/search_count)*100;
		txt = 'hit(%) : ' + percent.toPrecision(4) + '%';
		ctx.fillStyle="blue";
		ctx.fillText(txt,2300,2180);

}
