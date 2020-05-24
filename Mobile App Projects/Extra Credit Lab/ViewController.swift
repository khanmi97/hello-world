//
//  ViewController.swift
//  Lab8
//
//  Created by Md Khan on 4/28/20.
//  Copyright © 2020 Md Khan. All rights reserved.
//

import UIKit

class ViewController: UIViewController {

    
    
    @IBOutlet weak var monkeyImg: UIImageView!
    
    @IBOutlet weak var appleImg1: UIImageView!
    
    @IBOutlet weak var appleImg2: UIImageView!
    
    @IBOutlet weak var appleImg3: UIImageView!
    
    @IBOutlet weak var appleImg4: UIImageView!
    
    
    @IBOutlet weak var feedbackField: UITextField!
    
    
    @IBOutlet weak var timerLbl: UILabel!
    
    var timer: Timer?
    var secs = 15
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.
        timer = Timer()
        timer = Timer.scheduledTimer(timeInterval: 1, target: self, selector: #selector(ViewController.count), userInfo: nil, repeats: true)
    
    }
    
    
    
    @IBAction func up(_ sender: UIButton) {
        var frame = self.monkeyImg.frame
        frame.origin.y -= 10
        self.monkeyImg.frame = frame
        
        // hide apple images if monkey and it intersects
        if viewIntersectsView(monkeyImg, second_view: appleImg1)
        {
            self.appleImg1.isHidden = true
        }
        else if viewIntersectsView(monkeyImg, second_view: appleImg2)
        {
            self.appleImg2.isHidden = true
        }
        else if viewIntersectsView(monkeyImg, second_view: appleImg3)
        {
            self.appleImg3.isHidden = true
        }
        else if viewIntersectsView(monkeyImg, second_view: appleImg4)
        {
            self.appleImg4.isHidden = true
        }
        
        // Timer stops and monkey is the winner
        if self.appleImg1.isHidden && self.appleImg2.isHidden && self.appleImg3.isHidden && self.appleImg4.isHidden
        {
            timer?.invalidate()
            self.feedbackField.text = "Monkey is the Winner!"
        }
        
        
    }
    
    
    @IBAction func right(_ sender: UIButton) {
        var frame = self.monkeyImg.frame
        frame.origin.x += 10
        self.monkeyImg.frame = frame
        
        if viewIntersectsView(monkeyImg, second_view: appleImg1)
        {
            self.appleImg1.isHidden = true
        }
        else if viewIntersectsView(monkeyImg, second_view: appleImg2)
        {
            self.appleImg2.isHidden = true
        }
        else if viewIntersectsView(monkeyImg, second_view: appleImg3)
        {
            self.appleImg3.isHidden = true
        }
        else if viewIntersectsView(monkeyImg, second_view: appleImg4)
        {
            self.appleImg4.isHidden = true
        }
        
        if self.appleImg1.isHidden && self.appleImg2.isHidden && self.appleImg3.isHidden && self.appleImg4.isHidden
        {
            timer?.invalidate()
            self.feedbackField.text = "Monkey is the Winner!"
        }
        
    }
    
    
    @IBAction func left(_ sender: UIButton) {
        var frame = self.monkeyImg.frame
        frame.origin.x -= 10
        self.monkeyImg.frame = frame
        
        if viewIntersectsView(monkeyImg, second_view: appleImg1)
        {
            self.appleImg1.isHidden = true
        }
        else if viewIntersectsView(monkeyImg, second_view: appleImg2)
        {
            self.appleImg2.isHidden = true
        }
        else if viewIntersectsView(monkeyImg, second_view: appleImg3)
        {
            self.appleImg3.isHidden = true
        }
        else if viewIntersectsView(monkeyImg, second_view: appleImg4)
        {
            self.appleImg4.isHidden = true
        }
        
        if self.appleImg1.isHidden && self.appleImg2.isHidden && self.appleImg3.isHidden && self.appleImg4.isHidden
        {
            timer?.invalidate()
            self.feedbackField.text = "Monkey is the Winner!"
        }
    }
    
    
    @IBAction func down(_ sender: UIButton) {
        var frame = self.monkeyImg.frame
        frame.origin.y += 10
        self.monkeyImg.frame = frame
        
        if viewIntersectsView(monkeyImg, second_view: appleImg1)
        {
            self.appleImg1.isHidden = true
        }
        else if viewIntersectsView(monkeyImg, second_view: appleImg2)
        {
            self.appleImg2.isHidden = true
        }
        else if viewIntersectsView(monkeyImg, second_view: appleImg3)
        {
            self.appleImg3.isHidden = true
        }
        else if viewIntersectsView(monkeyImg, second_view: appleImg4)
        {
            self.appleImg4.isHidden = true
        }
        
        if self.appleImg1.isHidden && self.appleImg2.isHidden && self.appleImg3.isHidden && self.appleImg4.isHidden
        {
            timer?.invalidate()
            self.feedbackField.text = "Monkey is the Winner!"
        }
    }
    
    func viewIntersectsView(_ first_view: UIView, second_view: UIView) ->Bool
    {
        return first_view.frame.intersects(second_view.frame)
    }
    
    // countdowns to 0
    @objc func count()
    {
        secs -= 1
        self.timerLbl.text = String(secs)
        
        if secs == 0
        {
            timer?.invalidate()
            self.feedbackField.text = "GAME OVER"
            self.monkeyImg.isHidden = true
        }
    }
    
}

