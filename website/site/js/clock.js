window.onload = function() {

const hourHand = document.querySelector('.hourHand');
    const minuteHand = document.querySelector('.minuteHand');
    const secondHand = document.querySelector('.secondHand');
    const time = document.querySelector('.time');
    const clock = document.querySelector('.clock');

    function setDate(){
        const today = new Date();
        
        let second = today.getSeconds();
        const secondDeg = ((second / 60) * 360) + 360; 
        secondHand.style.transform = `rotate(${secondDeg}deg)`;
      
        
        let minute = today.getMinutes();
        const minuteDeg = ((minute / 60) * 360); 
        minuteHand.style.transform = `rotate(${minuteDeg}deg)`;

        const hour = today.getHours();
        const hourDeg = ((hour / 12 ) * 360 ); 
        hourHand.style.transform = `rotate(${hourDeg}deg)`;

		if (second < 10)
			second = "0" + second.toString()
		else
			second = second.toString()
		
		if (minute < 10)
			minute = "0" + minute.toString()
		else
			minute = minute.toString()
        
        time.innerHTML = '<span>' + '<strong>' + hour + '</strong>' + ' : ' + minute + ' : ' + '<small>' + second +'</small>'+ '</span>';
		if (hour == 00 && minute == "42" && second == "19")
			window.location.href = "https://github.com/TimourP"
    }

	window.addEventListener("mousemove", () => {
		if (Math.floor(Math.random() * 1000) == 17) {
			is_open = true;
			alert("Ça va?")
		}
		
	})
	setDate()
  
    setInterval(setDate, 1000);
 
}