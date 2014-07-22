% Batch sampling data analyzer.
clear all; clc;

%% Delay versus arrival rate for fixed number of servers.
num_servers_str = '10000';
num_servers = str2double(num_servers_str);
batch_size = round(log2(num_servers));
batch_size_str = num2str(batch_size);
arrival_rate_str = {'0.900000', '0.950000', '0.990000'};
arrival_rate = str2num(char(arrival_rate_str)).';
arrival_prob_str = '0.100000';
arrival_prob = str2double(arrival_prob_str);
total_time_str = {'100.000000', '200.000000', '300.000000'};
total_time = str2num(char(total_time_str)).';
probe_ratio_str = {'2.000000', '1.100000', '1.100000'};
probe_ratio = str2num(char(probe_ratio_str));
policy = {'mit', 'bs', 'bswf'};
policy_name = {'Pod', 'BS', 'BF'};
color = {'b', 'r', 'g'};
figure
hold on
for I_policy = 1:length(policy)
    actual_task_delay = zeros(1, length(arrival_rate));
    estimated_task_delay = zeros(1, length(arrival_rate));
    actual_batch_delay = zeros(1, length(arrival_rate));
    for I_rate = 1:length(arrival_rate)
        time_slot_length = batch_size*arrival_prob/arrival_rate(I_rate)...
            /num_servers;
        filename_suffix = ['n', num_servers_str, '_b', batch_size_str,...
            '_a', arrival_rate_str{I_rate}, '_p', arrival_prob_str,...
            '_t', total_time_str{I_rate}, '_r',...
            probe_ratio_str{I_policy}, '_',...
            policy{I_policy}, '_', probe_ratio_str{I_policy}];
        % Get task delay.
        task_delay_data = dlmread(['task_delays_', filename_suffix]);
        actual_task_delay(I_rate) = mean(task_delay_data(:, 2)...
            -task_delay_data(:, 1))*time_slot_length;
        % Get batch delay.
        batch_delay_data = dlmread(['batch_delays_', filename_suffix]);
        actual_batch_delay(I_rate) = mean(batch_delay_data(:, 2)...
            -batch_delay_data(:, 1))*time_slot_length;
    end
    % Plot task and batch delays.
    h1 = plot(arrival_rate, actual_task_delay, ['-o', color{I_policy}],...
        'LineWidth', 2);
    h2 = plot(arrival_rate, actual_batch_delay,...
        ['--s', color{I_policy}], 'LineWidth', 2);
    legend([h1, h2], [policy_name{I_policy}, ' d = ',...
        num2str(probe_ratio(I_policy)), ' average task delay'],...
        [policy_name{I_policy}, ' d = ', num2str(probe_ratio(I_policy)),...
        ' average batch delay']);
end
grid on
xlabel('Arrival rate')
ylabel('Average delay')

%% Evolution and histogram.
num_servers_str = '10000';
num_servers = str2double(num_servers_str);
batch_size = round(log2(num_servers));
batch_size_str = num2str(batch_size);
arrival_rate_str = '0.900000';
arrival_rate = str2double(arrival_rate_str);
arrival_prob_str = '0.100000';
arrival_prob = str2double(arrival_prob_str);
total_time_str = '100.000000';
total_time = str2double(total_time_str);
probe_ratio_str = '2.000000';
probe_ratio = str2double(probe_ratio_str);
policy = {'mit', 'bs', 'bswf'};
policy_name = {'Pod', 'BS', 'BF'};
color = {'b', 'r', 'g'};
for I_policy = 1:length(policy)
    % Read queue data.
    queue_data = dlmread(['queues_n', num_servers_str, '_b', batch_size_str,...
        '_a', arrival_rate_str, '_p', arrival_prob_str,...
        '_t', total_time_str, '_r', probe_ratio_str, '_',...
        policy{I_policy}, '_', probe_ratio_str]);
    % Draw evolution.
    figure
    hold all
    max_queue_length = size(queue_data, 2);
    num_time_slots = size(queue_data, 1);
    time_vector = (1:num_time_slots)/num_time_slots*total_time;
    for I = 1:max_queue_length
        plot(time_vector, queue_data(:, I)/num_servers);
    end
    grid on
    xlabel('Time')
    ylabel('Fraction of servers')
    % Calculate histogram using second half.
    s = mean(queue_data(round(num_time_slots/2):end, :)/num_servers, 1).';
    histogram(1:max_queue_length+1, I_policy) = [1; s]-[s; 0];
end
figure
h = bar(0:size(histogram, 1)-1, histogram, 'grouped');
grid on
legend(h, policy_name{1}, policy_name{2}, policy_name{3});
xlabel('Queue length')
ylabel('Fraction')
